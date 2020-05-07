"use strict";

angular
  .module("myApp.home", ["ngRoute"])

  .config([
    "$routeProvider",
    function($routeProvider) {
      $routeProvider.when("/home", {
        templateUrl: "home/home.html",
        controller: "homeCtrl"
      });
    }
  ])

  .controller("homeCtrl", function($scope, $http) {
    /** Defines and Constants */
    // Holds data about which nodes and edges are selected
    $scope.selectednodes = [];
    $scope.selectededges = [];

    // Constant d3 charge
    $scope.charge = -200;

    // Number of nodes
    $scope.nodecount = 0;

    // Graph data
    $scope.arch = "";
    $scope.dataset = {
      nodes: [],
      edges: []
    };
    $scope.node_data = {};

    // Execution Path Data
    $scope.path = [];
    $scope.node_dict = {};
    $scope.dummy = {};

    // Data on if there's an edge between two nodes
    $scope.linked = {};

    // Export obj
    $scope.exportObj = {};
    $scope.exportObj["json"] = "";
    $scope.exportObj["state"] = 0;
    $scope.exportObj["buttonText"] = "Show Export";

    // Undo data
    $scope.undoData = {};
    $scope.undoData["nodes"] = [];
    $scope.undoData["edges"] = [];

    // Window Size
    var HEADER_HEIGHT = 120;
    var w = window.innerWidth - 20;
    var h = window.innerHeight - HEADER_HEIGHT;

    // Node colors
    $scope.colors = d3.scale.category20b();
    $scope.next_color = 0;
    $scope.color_dict = {
      workload_generator: "gray:#7f7f7f",
      load_balancer: "turquoise:#17becf",
      webserver: "lightblue#9edae5",
      logicserver: "blue:#1f77b4",
      logic: "blue:#1f77b4",
      db: "green:#2ca02c",
      cache: "pink:#ff9896",
      caching: "pink:#ff9896",
      webserver: "cyan:#aec7e8"
    };
    /** END Defines and Constants */

    // this section is only used for generating request paths for the
    /* hotel_mgmt architecture.
    d3.json("./inputs/test_paths/hotel_mgmt/1.0.json", function(err, json) {
      console.log(json);

      var request_dict = {}
      // iterate all request path from json
      for(var i in json) {
        console.log(json[i]);

        var path = [];
        var json_path = json[i];
        var serve_time = 0;
        for(var j in json_path) {
            var node = json_path[j];
            serve_time += node.duration;
            path.push(node.localEndpoint.serviceName+".hotel_mgmt");
        }
        if(path in request_dict) {
            request_dict[path].push(serve_time);
        }else {
            request_dict[path] = [serve_time];
        }
      }
      console.log(request_dict);

      var request_cnt = 0;
      for(var key in request_dict) {
        var request = {};
        request["arch"] = "hotel_mgmt";
        request["path"] = key.split(",");
        request["delays"] = request_dict[key];
        console.log(request);
        var data = JSON.stringify(request, null, "\t");
        saveText(data, "request_"+request_cnt.toString(10)+".json");
        request_cnt += 1;
      }


      //$scope.architectures = json["arch"];
      //var formatted_json = JSON.stringify(json, null, "\t");
      //saveText(formatted_json, "formatted_file.json");
      //console.log(JSON.stringify(formatted_json));
    });
    */

    /** Architecture change code */
    d3.json("./inputs/arch_directory.json", function(err, json) {
      console.log(json);
      $scope.architectures = json["arch"];
    });

    /*
    // read request collection json file
    var request_path_dir = "./inputs/test_paths/" + $scope.arch +"/";
    d3.json(request_path_dir+"requests.json", function(err, json) {
      console.log(json);
      $scope.requests = json["requests"];
    });
    */

    // parse request json file and display execution path
    $scope.hasDrawTreeGraph = false;

    $scope.loadRequestFile = function(err, json) {
      if (!$scope.hasDrawTreeGraph) {
        $scope.drawTreeGraph();
        $scope.hasDrawTreeGraph = true;
      }

      $scope.path = [];
      var i;
      for (i = 0; i < json["path"].length; i++) {
        $scope.path.push($scope.node_dict[json["path"][i]]);
      }
      $scope.displayExecutionPath();
    };

    $scope.drawTreeGraph = function() {
      $scope.edge_container.selectAll("*").remove();
      $scope.node_container.selectAll("*").remove();

      $scope.force = d3.layout
        .force()
        .nodes($scope.dataset.nodes)
        .links($scope.dataset.edges)
        .size([w, h])
        .charge([$scope.charge])
        .linkDistance(function(d) {
          return 10 + (7 * (d.source.size + d.target.size)) / 2;
        });

      $scope.force.on("tick", $scope.treeTick);

      $scope.update();
      $scope.force.start();
      for (var i = 1000; i > 0; --i) $scope.force.tick();
      $scope.force.start();
      $scope.force.stop();
      $scope.zoomFit();
    };

    $scope.displayExecutionPath = function() {
      // compute end to end delay and request per second
      var delay = 0;
      for (var i = 0; i < $scope.path.length; i++) {
        var node_delay = parseInt(
          $scope.node_data[$scope.path[i][0]].delay,
          10
        );
        delay += node_delay;
      }

      // display info window
      if (!$scope.rps) {
        $scope.rps = 1;
      }
      document.getElementById("requestInfo").style.display = "block";

      document.getElementById("requestpersecond").innerHTML =
        $scope.rps + " request per second";
      document.getElementById("endtoenddelay").innerHTML = delay + " ms";
      $scope.edge_container.selectAll("#execution_path").remove();
      $scope.edge_container.selectAll("#execution_path_marker").remove();
      var lineFunction = d3.svg
        .line()
        .x(function(d) {
          return d.x;
        })
        .y(function(d) {
          return d.y;
        })
        .interpolate("linear");
      var lineGraph = $scope.edge_container
        .append("path")
        .attr("d", lineFunction($scope.path))
        .attr("stroke", "red")
        .attr("stroke-width", 2)
        .attr("id", "execution_path")
        .attr("fill", "none");

      console.log($scope.path);
      var startPoint = [$scope.path[0].x, $scope.path[0].y];

      // animation multiple marker
      for (var i = 0; i < 1000; i++) {
        var marker = $scope.edge_container
          .append("circle")
          .attr("r", 2)
          .style("fill", "green")
          .attr("id", "execution_path_marker")
          .attr("transform", "translate(" + startPoint + ")");
        transition(i);
      }

      function transition(i) {
        marker
          .transition()
          .duration(delay * 100)
          .delay((i * 1000) / $scope.rps)
          .attrTween("transform", translateAlong(lineGraph.node()));
      }

      function translateAlong(path) {
        var l = path.getTotalLength();
        return function(i) {
          return function(t) {
            var p = path.getPointAtLength(t * l);
            return "translate(" + p.x + "," + p.y + ")"; //Move marker
          };
        };
      }
    };
    
    $scope.loadAllRequestFile = async function(idx) {
      
      if (!$scope.hasDrawTreeGraph) {
        $scope.drawTreeGraph();
        $scope.hasDrawTreeGraph = true;
      }
      // load request file
      var path = [];
      var delays = [];
      d3.json(
        "./inputs/test_paths/" +
          $scope.archSelect +
          "/" +
          $scope.requests[idx] +
          ".json",
        await async function(err, json) {
          for (var i = 0; i < json["path"].length; i++) {
            path.push($scope.node_dict[json["path"][i]]);
          }

          if (json["delays"] === undefined) {
            var delay = 0;
            for (var i = 0; i < path.length; i++) {
              var node_delay = parseInt(
                $scope.node_data[path[i][0]].delay,
                10
              );
              delay += node_delay;
            }
            for (var i = 0; i < 1000; i++) {
              delays.push(delay * 100);
            }
          } else {
            for (var i = 0; i < json["delays"].length; i++) {
              delays.push(json["delays"][i]);
            }
          }
          all_paths.push(path);

          await $scope.displayAllExecutionPath(idx, path, delays);
          
        }
      );
    }

    $scope.displayAllExecutionPath = function(idx, path, delays) {
      // display info window
      if (!$scope.rps) {
        $scope.rps = 1;
      }

      var lineFunction = d3.svg
        .line()
        .x(function(d) {
          return d.x;
        })
        .y(function(d) {
          return d.y;
        })
        .interpolate("linear");

      var lineGraph = $scope.edge_container
        .append("path")
        .attr("d", lineFunction(path))
        .attr("stroke", $scope.pathColor[0])
        .attr("stroke-width", 2)
        .attr("id", "execution_path")
        .attr("fill", "none");

      path_line_graph.push(lineGraph.node());
    };

    $scope.treeTick = function(e) {
      var k = 6 * e.alpha;
      $scope.edge_container
        .selectAll("line")
        .each(function(d, i) {
          (d.source.x -= k * 3), (d.target.x += k * 5);
        })
        .attr("x1", function(d) {
          return d.source.x;
        })
        .attr("y1", function(d) {
          return d.source.y;
        })
        .attr("x2", function(d) {
          return d.target.x;
        })
        .attr("y2", function(d) {
          return d.target.y;
        });
      $scope.node_container
        .selectAll("circle")
        .attr("cx", function(d) {
          return d.x;
        })
        .attr("cy", function(d) {
          return d.y;
        });
    };

    $scope.tick = function() {
      $scope.edge_container
        .selectAll("line")
        .attr("x1", function(d) {
          return d.source.x;
        })
        .attr("y1", function(d) {
          return d.source.y;
        })
        .attr("x2", function(d) {
          return d.target.x;
        })
        .attr("y2", function(d) {
          return d.target.y;
        });
      $scope.node_container
        .selectAll("circle")
        .attr("cx", function(d) {
          return d.x;
        })
        .attr("cy", function(d) {
          return d.y;
        });
    };
    var div_array = [];
    $scope.fileLoad = function(err, json) {
      console.log(json);
      $scope.json = json;
      $scope.edge_container.selectAll("*").remove();
      $scope.node_container.selectAll("*").remove();
      $scope.arch = json.arch;
      $scope.dataset.nodes = [];
      $scope.dataset.edges = [];
      $scope.nodecount = 0;

      for (var i = 0; i < $scope.json.graph.length; i++) {
        var element = $scope.json.graph[i];
        console.log(element)
        if (element.node) {
          $scope.addnode(element.node);
          $scope.node_data[element.node] = element;
        }
      }
      for (var i = 0; i < $scope.json.graph.length; i++) {
        var element = $scope.json.graph[i];
        if (element.edge) {
          $scope.addedge(element.source, element.target);
        }
      }

      console.log($scope.dataset.edges);
      console.log($scope.dataset.nodes);
      console.log($scope.node_data);

      $scope.nodeCreate = false;
      $scope.edgeCreate = false;
      $scope.nodeDelete = false;
      $scope.edgeDelete = false;
      $scope.startNode = null;
      $scope.endNode = null;
      $scope.hasZoomed = false;
      $scope.force = d3.layout
        .force()
        .nodes($scope.dataset.nodes)
        .links($scope.dataset.edges)
        .size([w, h])
        .charge([$scope.charge])
        .linkDistance(function(d) {
          return 10 + (7 * (d.source.size + d.target.size)) / 2;
        });

      $scope.force.on("tick", $scope.tick);

      $scope.update();
      $scope.force.start();
      for (var i = 1000; i > 0; --i) $scope.force.tick();
      $scope.force.start();
      $scope.zoomFit();
      //$scope.force.stop();

      // construct node dict after graph building
      var i;
      for (i = 0; i < $scope.dataset.nodes.length; i++) {
        $scope.node_dict[$scope.dataset.nodes[i][0]] = $scope.dataset.nodes[i];
      }

      var nodes = $scope.dataset.nodes;
      for(i = 0; i < nodes.length; i++){
        var div = d3.select("body").append("div").style("opacity", 0);
        div_array.push(div);
      }
      console.log(div_array);
     
    };

    $scope.zoomFit = function() {
      var root = $scope.edge_container;
      var bounds = root.node().getBBox();
      var parent = root.node().parentElement;
      var fullWidth = parent.clientWidth || parent.parentNode.clientWidth,
        fullHeight = parent.clientHeight || parent.parentNode.clientHeight;
      var width = bounds.width,
        height = bounds.height;
      var midX = bounds.x + width / 2,
        midY = bounds.y + height / 2;
      if (width == 0 || height == 0) return; // nothing to fit
      var scale = 0.85 / Math.max(width / fullWidth, height / fullHeight);
      var translate = [
        fullWidth / 2 - scale * midX,
        fullHeight / 2 - scale * midY
      ];

      root
        .transition()
        .duration(300) // milliseconds
        .call(zoom.translate(translate).scale(scale).event);
    };
    var clearTimeOutvar;
    var request_path_flag = false;
    var nodeIndexMap = new Map();
    $scope.changeArch = function(newArch) {
      clearTimeout(clearTimeOutvar);
      request_path_flag = false;
      // clear previous div
      for(var i = 0; i < div_array.length; i++){
        div_array[i].classed("hidden", true);
      }
      div_array = [];
      console.log("Let find nodes!");
      $scope.requests = []; // clear previous requests
      $scope.archstep = 0;
      d3.json(
        "./inputs/" + $scope.archSelect + "_" + $scope.archstep + ".json",
        $scope.fileLoad
      );

      // console.log($scope.archSelect);
      
      $scope.hasDrawTreeGraph = false;
      // read request collection json file
      var request_path_dir = "./inputs/test_paths/" + $scope.archSelect + "/";
      function UrlExists(url) {
        var http = new XMLHttpRequest();
        http.open("HEAD", url, false);
        http.send();
        return http.status != 404;
      }
      if (!UrlExists(request_path_dir + "requests.json")) {
        return;
      }
      d3.json(request_path_dir + "requests.json", function(err, json) {
        // console.log(json);
        $scope.requests = json["requests"];
      });
      // console.log($scope.requests);
    };

    // depict new execution path
    $scope.changeExecutionPath = function(newRequest) {
      d3.json(
        "./inputs/test_paths/" +
          $scope.archSelect +
          "/" +
          $scope.requestSelect +
          ".json",
        $scope.loadRequestFile
      );
    };

    $scope.prevStep = function() {
      if ($scope.archstep > 0) {
        $scope.archstep = $scope.archstep - 1;
        d3.json(
          "./inputs/" + $scope.archSelect + "_" + $scope.archstep + ".json",
          $scope.fileLoad
        );
      }
    };

    $scope.nextStep = function() {
      $scope.archstep = $scope.archstep + 1;
      d3.json(
        "./inputs/" + $scope.archSelect + "_" + $scope.archstep + ".json",
        function(err, json) {
          if (json) {
            $scope.fileLoad(err, json);
          } else {
            $scope.archstep = $scope.archstep - 1;
          }
        }
      );
    };
    /** END architecture change code */

    /** Export Code */
    $scope.exportArch = function() {
      if ($scope.exportObj["state"] == 1) {
        //$scope.myWindow.close();
        $scope.exportObj["state"] = 0;
        $scope.exportObj["buttonText"] = "Show Export";
        return;
      }
      var result = {};
      result["arch"] = $scope.archSelect;
      result["step"] = $scope.archstep;
      result["graph"] = [];

      var edgecount = 1;
      var edges_encoded = new Map();

      $scope.dataset.edges.forEach(function(l) {
        if (!l.toberemoved) {
          const l_source = l.source[0];
          const l_target = l.target[0];
          const l_encode = l_source + "->" + l_target;
          const l_encode_reverse = l_target + "->" + l_source;
          if (edges_encoded[l_encode_reverse] == null) {
            var edgeobj = {};
            
            // fixed naming issue  
            var l_source_comp = l_source.split(".");
            edgeobj["source"] = l_source_comp.slice(1).join(".") + "." + l_source_comp[0];
            //edgeobj["source"] = l_source;
            console.log(l_source_comp.slice(1).join("."))
            
            var l_target_comp = l_target.split(".");
            edgeobj["target"] = l_target_comp.slice(1).join(".") + "." + l_target_comp[0];
            //edgeobj["target"] = l_target;
            // end fixing naming issue

            edgeobj["edge"] = "e" + edgecount;
            edges_encoded[l_encode] = "e" + edgecount;
            edgeobj["bidirectional"] = "no";
            edgecount = edgecount + 1;
            result["graph"].push(edgeobj);
          } else {
            var j = 0;
            for (; j < result["graph"].length; j++) {
              if (
                result["graph"][j]["edge"] === edges_encoded[l_encode_reverse]
              ) {
                break;
              }
            }

            result["graph"][j]["bidirectional"] = "yes";
          }
        }
      });

      $scope.dataset.nodes.forEach(function(n) {
        if (!n.toberemoved) {
          var nodeobj = {};
          var cur_node = $scope.node_data[n[0]];
          console.log(n);

        
          // fixed naming issue  
          var name_comp = cur_node.node.split(".");
          console.log(name_comp);
          nodeobj["node"] = name_comp.slice(1).join(".") + "." + name_comp[0];

                      
          nodeobj["bidirectional"] = cur_node.bidirectional;

          //nodeobj["node"] = cur_node.node;

          nodeobj["package"] = cur_node.package;
          nodeobj["metadata"] = cur_node.metadata;
          nodeobj["timestamp"] = cur_node.timestamp;
          nodeobj["delay"] = cur_node.delay;
          result["graph"].push(nodeobj);
        }
      });
      //console.log(result)

      $scope.exportObj["json"] = JSON.stringify(result, null, "\t");
      $scope.exportObj["state"] = 1;
      $scope.exportObj["buttonText"] = "Hide Export";
      //opens the export json in a new window, but only works if there is no popup blocker...
      var data = $scope.exportObj["json"];
      //$scope.myWindow = window.open("data:text/html," + encodeURIComponent(data), "_blank", "width=500, height=200");
      //$scope.myWindow.focus();
      saveText(data, $scope.arch + "_0.json");
      console.log(JSON.stringify(result));
    };
    /** END Export Code */

    /** Defines Visualization container and container interactions */
    $scope.trans = [0, 0];
    $scope.scale = 0;

    //if user has scrolled, this returns the actual x, y on screen given translation and scaling matrices
    function getScreenCoords(x, y, translate, scale) {
      var xn = translate[0] + x * scale;
      var yn = translate[1] + y * scale;
      return { x: xn, y: yn };
    }

    /**Zooming in and out**/
    //lets user zoom in and out only if is not in edge create mode
    function zoomed() {
      if ($scope.edgeCreate) return;
      $scope.hasZoomed = true;
      $scope.edge_container.attr(
        "transform",
        "translate(" + d3.event.translate + ")scale(" + d3.event.scale + ")"
      );
      $scope.node_container.attr(
        "transform",
        "translate(" + d3.event.translate + ")scale(" + d3.event.scale + ")"
      );
      $scope.trans = d3.event.translate;
      $scope.scale = d3.event.scale;
    }

    var zoom = d3.behavior
      .zoom()
      .scaleExtent([0.1, 30])
      .on("zoom", zoomed);
    /**End zoom section**/

    /**Functions for dragging the graph around**/
    function dragstarted(d) {
      if ($scope.edgeCreate) return;

      $scope.force.start();
    }

    function dragged(d) {
      if ($scope.edgeCreate) return;
      d.px += d3.event.dx;
      d.py += d3.event.dy;
      d.x += d3.event.dx;
      d.y += d3.event.dy;
      $scope.tick();
    }

    function dragended(d) {
      if ($scope.edgeCreate) return;
      d.fixed = true;
      $scope.tick();
      $scope.force.resume();
      $scope.force.start();
    }

    $scope.drag = d3.behavior
      .drag()
      .origin(function(d) {
        return d;
      })
      .on("dragstart", dragstarted)
      .on("drag", dragged)
      .on("dragend", dragended);

    /**End drag section**/

    // SVG
    $scope.svg = d3
      .select("body")
      .append("svg")
      .attr("width", w)
      .attr("height", h)
      .append("g")
      .call(zoom)
      .on("mousedown.zoom", null)
      .on("dblclick.zoom", null);

    $scope.svg
      .append("svg:defs")
      .append("svg:marker")
      .attr("id", "end-arrow")
      .attr("viewBox", "0 -5 10 10")
      .attr("refX", 6)
      .attr("markerWidth", 3)
      .attr("markerHeight", 3)
      .attr("orient", "auto")
      .append("svg:path")
      .attr("d", "M0,-5L10,0L0,5")
      .attr("fill", "#000");

    $scope.svg
      .append("svg:defs")
      .append("svg:marker")
      .attr("id", "triangle")
      .attr("refX", 6)
      .attr("refY", 6)
      .attr("markerWidth", 30)
      .attr("markerHeight", 30)
      .attr("markerUnits", "userSpaceOnUse")
      .attr("orient", "auto")
      .append("path")
      .attr("d", "M 0 0 12 6 0 12 3 6")
      .style("fill", "black");

    $scope.svg
      .append("svg:defs")
      .append("svg:marker")
      .attr("id", "arrow")
      .attr("viewBox", "0 -5 10 10")
      .attr("refX", -20)
      .attr("markerWidth", 5)
      .attr("markerHeight", 5)
      .attr("orient", "auto")
      .append("svg:path")
      .attr("d", "M0,-5L10,0L0,5");

    var rect = $scope.svg
      .append("rect")
      .attr("width", w)
      .attr("height", h)
      .style("fill", "none")
      .style("pointer-events", "all")
      //mousemove only relevant for edge creation, so if there is no startNode for beginning of edge set, return
      .on("mousemove", function() {
        if (!$scope.startNode) return;
        var coords = $scope.hasZoomed
          ? getScreenCoords(
              $scope.startNode.x,
              $scope.startNode.y,
              $scope.trans,
              $scope.scale
            )
          : { x: $scope.startNode.x, y: $scope.startNode.y };

        // update drag line
        $scope.drag_line.attr(
          "d",
          "M" +
            coords.x +
            "," +
            coords.y +
            "L" +
            d3.mouse(this)[0] +
            "," +
            d3.mouse(this)[1]
        );
      })
      //clicking creates a node at that point if in nodeCreate mode, otherwise can drag the graph around
      .on("click", function(s) {
        if ($scope.nodeCreate) {
          $scope.createNode(d3.mouse(this));
        }

        d3.selectAll("circle")
          .transition()
          .duration(300)
          .style("opacity", function(n) {
            return 1;
          });

        d3.selectAll("line")
          .transition()
          .duration(300)
          .style("opacity", function(l) {
            return 1;
          });
      });

    $scope.edge_container = $scope.svg.append("g");
    $scope.node_container = $scope.svg.append("g");
    //drag_line that appears when creating an edge to show what 2 nodes you are linking
    $scope.drag_line = $scope.svg
      .append("svg:path")
      .attr("class", "link dragline hidden")
      .attr("d", "M0,0L0,0")
      .attr("stroke-width", 2)
      .attr("stroke", "black");

    //defines the edges in the graph
    $scope.buildEdges = function() {
      return (
        $scope.edge_container
          .selectAll("line")
          .data($scope.dataset.edges)
          .enter()
          .append("line")
          .style("stroke", function(d) {
            return "#ccc";
          })
          .style("stroke-width", 2)
          .attr("x1", function(d) {
            return d.source.x;
          })
          .attr("marker-end", function(d) {
            return "url(#triangle)";
          })
          .attr("y1", function(d) {
            return d.source.y;
          })
          .attr("x2", function(d) {
            return d.target.x;
          })
          .attr("y2", function(d) {
            return d.target.y;
          })
          //click is only relevant if in edge deletion mode
          .on("click", function(d) {
            if ($scope.edgeDelete) {
              $scope.cleanUndo(); //can only undo 1 delete at a time, so clean up all existing to be deleted objects
              this.style["visibility"] = "hidden"; //hide it in case user wants to cancel delete
              this.__data__.toberemoved = true; //sets it to be removed
              $scope.undoData["nodes"] = [];
              $scope.undoData["edges"] = [this];
              $scope.subtractSize([this]); //nodes connected to it need to be made smaller
              d3.selectAll("circle")
                .data($scope.dataset.nodes)
                .attr("r", function(d) {
                  return Math.sqrt(d.size) * 2;
                });
              $scope.edgeDelete = false;
              document.getElementById("button3").style.borderStyle = "";
            }
          })
      );
    };

    /*
    //nodes in the graph
    $scope.buildNodes = function() {
      return (
        $scope.node_container
          .selectAll("circle")
          .data($scope.dataset.nodes)
          .enter()
          .append("circle")
          .attr("r", function(d, i) {
            return Math.sqrt(d.size) * 2;

          })
          //fill is based on the application, zone, name such that they are grouped by color
          .style("fill", function(d, i) {
            var names = d[0].split(".");
            //TODO: changed to 3 because of twitter format, change back to 4 for spigo graphs
            //if (names.length < 3) {
            if (names.length < 4) {
              return $scope.colors(0);
            } else {
              //TODO: is 3 for normal graphs
              var app = $scope.removeNum(names[3]);
              //TODO: changed to 2 for twitter graphs
              //var app = $scope.removeNum(names[2]);
              var zone = names[2];
              //TODO: changed to 1 for twitter graphs
              //var zone = names[1];
              var color_ctr = 0;
              if (app in $scope.color_dict) {
                var colorObj = $scope.color_dict[app];
                if (zone in colorObj["mapping"]) {
                  return $scope.colors(colorObj["mapping"][zone]);
                } else {
                  color_ctr = colorObj["counter"] + 1;
                  colorObj["counter"] = color_ctr; //1 - colorObj["counter"];
                  var shade = colorObj["base"] + colorObj["counter"];
                  colorObj["mapping"][zone] = shade;
                  return $scope.colors(shade);
                }
              } else {
                var color1 = $scope.colors($scope.next_color);
                var color2 = $scope.colors($scope.next_color + 1);
                var color3 = $scope.colors($scope.next_color + 2);
                var colorObj = {};
                colorObj["mapping"] = {};
                colorObj["mapping"][zone] = $scope.next_color;
                colorObj["counter"] = 1;
                colorObj["base"] = $scope.next_color;
                $scope.color_dict[app] = colorObj;
                if ($scope.next_color < 20) {
                  $scope.next_color = ($scope.next_color + 4) % 20;
                } else {
                  $scope.colors = d3.scale.category20c();
                  $scope.next_color = -4;
                  $scope.next_color = ($scope.next_color + 4) % 20;
                }
                return $scope.colors(colorObj["mapping"][zone]);
              }
            }
          })
          .style("stroke", "black")
          .style("stroke-width", 1)
          .on("click", function(d) {
            //like in edge delete, we need to ensure only one thing can be undo'd at a time
            //we save the node until we are sure the user will not undo it
            if ($scope.nodeDelete) {
              $scope.cleanUndo();

              var toRemove = $scope.removeLinks(d[0]);
              $scope.subtractSize(toRemove);
              d3.selectAll("circle")
                .data($scope.dataset.nodes)
                .attr("r", function(d) {
                  return Math.sqrt(d.size) * 2;
                });
              $scope.undoData["edges"] = toRemove;
              $scope.undoData["nodes"] = [this];
              this.style["visibility"] = "hidden";
              this.__data__.toberemoved = true;
              $scope.nodeDelete = false;
              document.getElementById("button4").style.borderStyle = "";
              d3.select("#tooltip").classed("hidden", true);
            } else if ($scope.edgeCreate) {
              if (!$scope.startNode) {
                $scope.startNode = d;
                //in case user has zoomed, need to get real coordinates of the point based on transformation
                var coords = $scope.hasZoomed
                  ? getScreenCoords(d.x, d.y, $scope.trans, $scope.scale)
                  : { x: d.x, y: d.y };

                // reposition drag line
                $scope.drag_line
                  .style("marker-end", "url(#end-arrow)")
                  .classed("hidden", false)
                  .attr(
                    "d",
                    "M" +
                      coords.x +
                      "," +
                      coords.y +
                      "L" +
                      coords.x +
                      "," +
                      coords.y
                  );
              } else {
                //if there is already a start node, connect the drag_line to the end node only if start node != end node

                //hide the drag_line when a real edge is made
                $scope.drag_line
                  .classed("hidden", true)
                  .style("marker-end", "");

                // check for drag-to-self
                $scope.endNode = d;
                if ($scope.startNode === $scope.endNode) {
                  return;
                }
                $scope.addedge($scope.startNode[0], $scope.endNode[0]);
                $scope.buildEdges();
                $scope.links = $scope.edge_container.selectAll("line");
                $scope.resetButtons();
              }
            } else {
              //highlight the node and all edges connected to it
              $scope.linked = {};
              for (var i = 0; i < $scope.force.nodes().length; i++) {
                $scope.linked[
                  $scope.force.nodes()[i].index +
                    "," +
                    $scope.force.nodes()[i].index
                ] = 1;
              }
              for (var j = 0; j < $scope.force.links().length; j++) {
                $scope.linked[
                  $scope.force.links()[j].source.index +
                    "," +
                    $scope.force.links()[j].target.index
                ] = 1;
              }
              const cur = d3.select(this).node().__data__;
              d3.selectAll("circle")
                .transition()
                .duration(100)
                .style("opacity", o => {
                  return neighboring(cur, o) | neighboring(o, cur) ? 1 : 0.2;
                });

              d3.selectAll("line")
                .transition()
                .duration(100)
                .style("opacity", function(o) {
                  return (cur.index === o.source.index) |
                    (cur.index === o.target.index)
                    ? 1
                    : 0.2;
                });
            }
          })
          .on("mouseover", function(d) {
            //Get this node's x/y values, then augment for the tooltip
            var dependencies = $scope.getDependencies(d[0]);
            var dependList = "";
            dependencies.forEach(function(e) {
              dependList += e.__data__.target[0] + "<br>";
            });
            var xPosition = w - 500;
            var yPosition = 60;
            //Update the tooltip position and value
            d3.select("#tooltip")
              .style("left", xPosition + "px")
              .style("top", yPosition + "px")
              .select("#value")
              .style("z-value", 9999)
              .html(
                "Name: " + d + "<br>" + "Dependencies: " + "<br>" + dependList
              );
            //Show the tooltip
            d3.select("#tooltip").classed("hidden", false);
            d3.event.stopPropagation();
          })
          .on("mouseleave", function() {
            //Hide the tooltip
            d3.select("#tooltip").classed("hidden", true);
          })
          .call($scope.drag)
          .attr("cx", function(d) {
            return d.x;
          })
          .attr("cy", function(d) {
            return d.y;
          })
      );
    };
    */

    //nodes in the graph
    $scope.buildNodes = function() {
      return (
        $scope.node_container
          .selectAll("circle")
          .data($scope.dataset.nodes)
          .enter()
          .append("circle")
          .attr("r", function(d, i) {
            return Math.sqrt(d.size) * 2.4;
          })
          //fill is based on the server package type
          .style("fill", function(d, i) {
            var pkg = $scope.node_data[d[0]].package;
            if (pkg in $scope.color_dict) {
              return $scope.color_dict[pkg].split(":")[0];
            } else {
              return "#000000"; // as black
            }
          })
          .style("stroke", "black")
          .style("stroke-width", 1)
          .on("click", function(d) {
            //like in edge delete, we need to ensure only one thing can be undo'd at a time
            //we save the node until we are sure the user will not undo it
            if ($scope.nodeDelete) {
              $scope.cleanUndo();

              var toRemove = $scope.removeLinks(d[0]);
              $scope.subtractSize(toRemove);
              d3.selectAll("circle")
                .data($scope.dataset.nodes)
                .attr("r", function(d) {
                  return Math.sqrt(d.size) * 2;
                });
              $scope.undoData["edges"] = toRemove;
              $scope.undoData["nodes"] = [this];
              this.style["visibility"] = "hidden";
              this.__data__.toberemoved = true;
              $scope.nodeDelete = false;
              document.getElementById("button4").style.borderStyle = "";
              d3.select("#tooltip").classed("hidden", true);
            } else if ($scope.edgeCreate) {
              if (!$scope.startNode) {
                $scope.startNode = d;
                //in case user has zoomed, need to get real coordinates of the point based on transformation
                var coords = $scope.hasZoomed
                  ? getScreenCoords(d.x, d.y, $scope.trans, $scope.scale)
                  : { x: d.x, y: d.y };

                // reposition drag line
                $scope.drag_line
                  .style("marker-end", "url(#end-arrow)")
                  .classed("hidden", false)
                  .attr(
                    "d",
                    "M" +
                      coords.x +
                      "," +
                      coords.y +
                      "L" +
                      coords.x +
                      "," +
                      coords.y
                  );
              } else {
                //if there is already a start node, connect the drag_line to the end node only if start node != end node

                //hide the drag_line when a real edge is made
                $scope.drag_line
                  .classed("hidden", true)
                  .style("marker-end", "");

                // check for drag-to-self
                $scope.endNode = d;
                if ($scope.startNode === $scope.endNode) {
                  return;
                }
                $scope.addedge($scope.startNode[0], $scope.endNode[0]);
                $scope.buildEdges();
                $scope.links = $scope.edge_container.selectAll("line");
                $scope.resetButtons();
              }
            } else {
              //highlight the node and all edges connected to it
              $scope.linked = {};
              for (var i = 0; i < $scope.force.nodes().length; i++) {
                $scope.linked[
                  $scope.force.nodes()[i].index +
                    "," +
                    $scope.force.nodes()[i].index
                ] = 1;
              }
              for (var j = 0; j < $scope.force.links().length; j++) {
                $scope.linked[
                  $scope.force.links()[j].source.index +
                    "," +
                    $scope.force.links()[j].target.index
                ] = 1;
              }
              const cur = d3.select(this).node().__data__;
              d3.selectAll("circle")
                .transition()
                .duration(100)
                .style("opacity", o => {
                  return neighboring(cur, o) | neighboring(o, cur) ? 1 : 0.2;
                });

              d3.selectAll("line")
                .transition()
                .duration(100)
                .style("opacity", function(o) {
                  return (cur.index === o.source.index) |
                    (cur.index === o.target.index)
                    ? 1
                    : 0.2;
                });
            }
          })
          .on("mouseover", function(d) {
            if(!data_shown){

              // $scope.startNode = d;
            console.log(d.index);
            //in case user has zoomed, need to get real coordinates of the point based on transformation
            var coords = $scope.hasZoomed
              ? getScreenCoords(d.x, d.y, $scope.trans, $scope.scale)
              : { x: d.x, y: d.y };
            // console.log(coords.x)
            // console.log(coords.y)
            
            //Get this node's x/y values, then augment for the tooltip
            var dependencies = $scope.getDependencies(d[0]);
            var dependList = "";
            dependencies.forEach(function(e) {
              dependList += e.__data__.target[0] + "<br>";
            });
            // var xPosition = w - 500;
            // var yPosition = 60;
            var xPosition = coords.x;
            var yPosition = coords.y;
            var div = div_array[d.index];
            div.style("opacity", .6);	
            div.style("position", "fixed")
            .style("border", "0.5px")
            .style("border-radius", "5px")
            .style("background", "lightsteelblue")
            if(request_path_flag && throughput_display[temp_index][d.index] != 0){
              div.style("left", coords.x + "px")
                .style("top", coords.y - 5 + "px");
              div.html(String(d).substr(String(d).lastIndexOf(".") + 1) + "<br>" + "Throughput: " + throughput_display[temp_index][d.index]  + " qps" + "<br>" + "Latency: " + delay_display[temp_index][d.index]/1000000 + "ms");
            }else{
              div.style("left", coords.x + "px")
                .style("top", coords.y + 50 + "px");
              div.html(String(d).substr(String(d).lastIndexOf(".") + 1) + "<br>");
            }
            div.classed("hidden", false);

            
            // d3.select("#tooltip")
            //   .style("position", "fixed")
            //   .style("left", xPosition + "px")
            //   .style("top", yPosition + "px")
            //   .select("#value")
            //   .style("z-value", 9999)
            //   .html(
            //     // "Name: " + d + "<br>" + "Dependencies: " + "<br>" + dependList + "Throughput: " 
            //     "Name: " + d + "<br>" + "Throughput: " + "qps" + "<br>" + "Tatency: "
            //   );
            // //Show the tooltip
            // d3.select("#tooltip").classed("hidden", false);
            d3.event.stopPropagation();
            }
          })
          .on("mouseleave", function(d) {
            if(!data_shown){
              //Hide the tooltip
              div_array[d.index].classed("hidden", true);
              // d3.select("#tooltip").classed("hidden", true);
            } 
          })
          .call($scope.drag)
          .attr("cx", function(d) {
            return d.x;
          })
          .attr("cy", function(d) {
            return d.y;
          })
      );
    };
    
    $scope.ShowData = function() {
      data_shown = !data_shown;
      console.log("new");
      if(data_shown){
        var nodes = $scope.dataset.nodes;
        var i;
        for (i = 0; i < nodes.length; i++) { 
          var coords = $scope.hasZoomed
                ? getScreenCoords(nodes[i].px, nodes[i].py, $scope.trans, $scope.scale)
                : { x: d.x, y: d.y };
          var div = div_array[i];
          div.style("opacity", .6);	
          div.style("position", "fixed")
          .style("border", "0.5px")
          .style("border-radius", "5px")
          .style("background", "lightsteelblue")
          if(request_path_flag && throughput_display[temp_index][i] != 0){
            div.style("left", coords.x + "px")
            .style("top", coords.y - 5 + "px");
            div.html(String(nodes[i][0]).substr(String(nodes[i][0]).lastIndexOf(".") + 1) + "<br>" + "Throughput: " + throughput_display[temp_index][i]  + " qps" + "<br>" + "Latency: " + delay_display[temp_index][i]/1000000 + "ms");
          }else{
            div.style("left", coords.x + "px")
            .style("top", coords.y + 50 + "px");
            div.html(String(nodes[i][0]).substr(String(nodes[i][0]).lastIndexOf(".") + 1) + "<br>");
          }
          var tip = "#tooltip".concat(i);
          var val = "#value".concat(i);
          div.classed("hidden", false);
        }
      }else{
        var i;
        for (i = 0; i < div_array.length; i++) {
          div_array[i].classed("hidden", true);
        }
      }
    }

    /** END Defines Visualization container and container interactions */

    /** Update Code */
    $scope.update = function() {
      $scope.edge_container.selectAll("*").remove();
      $scope.node_container.selectAll("*").remove();

      $scope.links = $scope.buildEdges();

      $scope.nodes = $scope.buildNodes();
    };
    /** END Update Code */

    /**Node Creation**/

    //when the create node button is clicked, trigger this function to set mode and change button shape
    $scope.createNodeSelected = function(e) {
      $scope.resetButtons();
      $scope.nodeCreate = true;
      document.getElementById(e.currentTarget.id).style.borderStyle = "inset";
      document.getElementById("nodeFormContainer").style.display = "block";
    };

    //upon node creation form submission trigger this
    $scope.newNode = function() {
      var form = document.getElementById("nodeFormContainer");
      var error = document.getElementById("nodeFormError");
      //check that all fields of form are filled in
      if (
        typeof $scope.node === "undefined" ||
        typeof $scope.node.name === "undefined" ||
        typeof $scope.node.arch === "undefined" ||
        typeof $scope.node.region === "undefined" ||
        typeof $scope.node.zone == "undefined" ||
        typeof $scope.node.package == "undefined" ||
        typeof $scope.node.delay == "undefined"
      ) {
        //display an error if not all filled
        error.style.display = "block";
      } else {
        form.style.display = "none";
        error.style.display = "none";
        // set the name of architecture
        $scope.arch = $scope.node.arch;
        $scope.archSelect = $scope.arch;
        //create name by simple concatenation
        var name =
          
          $scope.node.arch +
          "." +
          $scope.node.region +
          "." +
          $scope.node.zone +
          "." + 
          $scope.node.name;
        var n = $scope.addnode(name);

        // construct node data and add it into node_data dictionary
        var element = {
          node: name,
          package: $scope.node.package,
          delay: $scope.node.delay
        };
        $scope.node_data[element.node] = element;
        console.log($scope.node_data);
        //clear the form
        var defaultForm = {
          name: undefined,
          arch: undefined,
          region: undefined,
          zone: undefined,
          package: undefined,
          delay: undefined
        };

        $scope.node = angular.copy(defaultForm);
        document.getElementById("button2").style.borderStyle = "";
      }
    };

    //after form has been submitted, this method lets users create a node at where they click
    //accounts for zoom by transforming the coordinates if user has zoomed
    $scope.createNode = function(point) {
      var n = $scope.dataset.nodes.length - 1;
      $scope.dataset.nodes[n].x = $scope.hasZoomed
        ? (point[0] - $scope.trans[0]) / $scope.scale
        : point[0];
      $scope.dataset.nodes[n].y = $scope.hasZoomed
        ? (point[1] - $scope.trans[1]) / $scope.scale
        : point[1];
      $scope.buildNodes();
      $scope.nodes = $scope.node_container.selectAll("circle");
      $scope.resetButtons();
    };

    /**End Node Creation**/

    /**Edge Creation**/
    //method triggered when user selects create edge button. Note that the rest of edge creation logic is in on click for nodes
    //once mode is set to edge creation, if user clicks on a node the drag_line will appear. clicking on second different node
    //will result in an edge

    $scope.createEdgeSelected = function(e) {
      $scope.resetButtons();
      if ($scope.dataset.nodes.length > 1) {
        document.getElementById(e.currentTarget.id).style.borderStyle = "inset";

        $scope.edgeCreate = true;
      }
    };

    $scope.displayRequestPathSelected = function(e) {
      $scope.dummy["arch"] = $scope.arch;
      console.log($scope.rps);
    };

    $scope.pathColor = ["red", "blue", "yellow", "purple", "pink", "maroon", "orange", "olive", "#FFE87C", "lightblue"];
    $scope.markColor = ["blue", "yellow", "purple", "pink", "maroon", "orange", "olive", "#FFE87C", "lightblue"];




    var data_shown = false;
    var throughput_display_2_tier_ngx_8 = [[1000, 1000, 1000], 
    [1000, 1000, 1000], [1000, 1000, 1000], 
    [1000, 1000, 1000], [1000, 1000, 1000], 
    [1000, 1000, 1000]
    , [1000, 1000, 1000],
     [1000, 1000, 1000], [1000, 1000, 1000], 
     [1000, 1000, 1000], [1000, 1000, 1000], 
     [1000, 1000, 1000], [1000, 1000, 1000], 
     [1000, 1000, 1000]
    , [1000, 1000, 1000], [1000, 1000, 1000], 
    [1000, 1000, 1000], [1000, 1000, 1000], 
    [1000, 1000, 1000], [1000, 1000, 1000]];
    var delay_display_2_tier_ngx_8 = [[314939, 65076, 38554], 
    [314233, 65076, 40219], [314397, 65305, 38326],  [315546, 65264, 38320], 
    [313376, 65059, 41607], [312764, 65123, 39089], [316869, 65311, 41067], 
    [315125, 65170, 40271], [315551, 65471, 40446],  [314152, 60436, 40514],
     [314564, 52564, 39440], [312921, 53613, 40497], [315405, 65325, 40411], 
     [314528, 61100, 40994], [315802, 65241, 39818], [315690, 65012, 39789], 
     [315016, 60587, 43004], [316054, 65219, 41577], [315106, 62015, 40731], 
     [317052, 65568, 41280]];
    
    var temp_index = -1;

    var throughput_map_2_tier_ngx_8 = new Map();
    throughput_map_2_tier_ngx_8.set("client",[1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_2_tier_ngx_8.set("nginx",[1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_2_tier_ngx_8.set("memcached",[1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    var delay_map_2_tier_ngx_8 = new Map();
    
    
    delay_map_2_tier_ngx_8.set("client",[314939,314233,314397,315546, 313376,312764, 316869, 315125, 315551, 314152,314564, 312921,315405,314528, 315802, 315690, 315016, 316054, 315106, 317052]);
    delay_map_2_tier_ngx_8.set("nginx",[65076, 65305, 65264, 65059, 65123, 65311, 65170, 65471, 60436, 52564, 53613, 65325, 61100, 65241, 65012,60587, 65219, 62015,65568, 65076]);
    delay_map_2_tier_ngx_8.set("memcached",[ 38554, 40219, 38326, 38320, 39089, 41067, 40271, 40446, 40514, 39440, 40497, 40411, 40994, 39818, 39789, 43004, 41577, 40731, 41280,  40280]);


    var throughput_map_social_network  = new Map();
    throughput_map_social_network.set("client", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000])
    throughput_map_social_network.set("load_balancer", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("nginx", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("media", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("unique_id", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("url_shorten", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("text", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("user", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("compose_post", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("post_storage", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("memcached_1", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("mongodb_1", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("user_timeline", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("redis_0", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("mongoldb_2", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("rabbitmq", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("write_home_timeline", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("redis_1", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("social_graph", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("memcached_2", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);
    throughput_map_social_network.set("mongodb_3", [1000,1000,1000,1000,1000,1000,1000,1000,1000,1000]);

    var delay_map_social_network = new Map();
    delay_map_social_network.set("client", [992438, 1008449, 1004157, 992361, 1021163, 1008291,1005949, 1009857, 1009996, 985039]);
    delay_map_social_network.set("load_balancer", [1014, 1000, 1000, 1000, 1071, 1000,1084, 1000, 1024, 1074]);
    delay_map_social_network.set("nginx", [65204, 65179, 54403, 54025, 65097, 65395,65039,65316, 65147, 65188]);
    delay_map_social_network.set("media", [7152, 7228, 7003, 4740, 6847, 7088, 6841, 7040, 6728, 6848]);
    delay_map_social_network.set("unique_id", [4532, 4651, 3688, 4643, 4694, 4568, 4636, 4497, 4627, 4550]);
    delay_map_social_network.set("url_shorten", [3794, 3795, 3638, 3638, 3778, 3793, 3724, 3787, 3634, 3703]);
    delay_map_social_network.set("text", [4139, 4127, 4083, 4046, 4073, 4078, 4119, 4124, 4065, 4108]);
    delay_map_social_network.set("user", [2741, 2754, 2824, 2811, 2775, 2813, 2736, 2794, 2911, 2696]);
    delay_map_social_network.set("compose_post", [9793, 9088, 8875, 9268, 8905, 9231, 9132, 9274, 9418, 9247]);
    delay_map_social_network.set("post_storage", [11574, 10621, 11232, 10954, 11499, 10768,11118,10894,11123,11543]);
    delay_map_social_network.set("memcached_1", [37286, 38331, 37975, 38339, 38051, 38159,38972,39115,38898,38010]);
    delay_map_social_network.set("mongodb_1", [366913, 380954, 388202, 378497, 372835,385163,370845,368353,375005,368942]);
    delay_map_social_network.set("user_timeline", [5077, 4965, 4957, 5187, 5018,4968, 5093, 5314, 4932, 5111]);
    delay_map_social_network.set("redis_0", [10550, 9965, 9954, 9944, 10083,10187,10471, 10115,10316,10336]);
    delay_map_social_network.set("mongoldb_2", [381813, 382449, 383041, 371938, 381270,390908,383843,377306,387495,383765]);
    delay_map_social_network.set("rabbitmq", [13761, 13752, 14017, 13028, 13878,13890, 13883,13955,14393,13551]);
    delay_map_social_network.set("write_home_timeline", [8765, 8442, 8779, 8470, 8901,8945, 8949, 8787, 8787,8606]);
    delay_map_social_network.set("redis_1", [10253, 9863, 10095, 10378,10218,9922, 10340,10341,10314, 9885]);
    delay_map_social_network.set("social_graph", [7900, 7864, 7753, 8062, 7655,7761, 8084, 7656, 8178, 7835]);
    delay_map_social_network.set("memcached_2", [38604, 38166, 38149, 39279, 39076,39084,38410, 37807, 39038,37824]);
    delay_map_social_network.set("mongodb_3", [373509, 384660, 387698, 364346, 376912,382762,374392,382551, 388041, 357732]);

    var throughput_map = new Map();
    var delay_map = new Map();

    var throughput_display = [];
    var delay_display = [];
    var clearTimeOutvar1;


    // $scope.callback = function(){
    //   console.log("Try");
    // }
    var all_paths
    var path_line_graph

    $scope.AllRequestPathSelected = async function(e) {
      // Create node index map a map;
      nodeIndexMap = new Map();
      for(var i = 0; i < $scope.dataset.nodes.length; i++){
        nodeIndexMap.set(String($scope.dataset.nodes[i][0]).substr(String($scope.dataset.nodes[i][0]).lastIndexOf(".") + 1),i);
      }
      console.log("node index map set");
      console.log(nodeIndexMap);

      all_paths = new Array();
      path_line_graph = new Array();

      throughput_display = [];
      delay_display = [];

      $scope.edge_container.selectAll("#execution_path").remove();
      $scope.edge_container.selectAll("#execution_path_marker").remove();
      $scope.hasDrawTreeGraph = false;
      // clear the previous timeout
      clearTimeout(clearTimeOutvar);
      clearTimeout(clearTimeOutvar1);

      for(var i = 0; i < $scope.requests.length; i++) {
        await $scope.loadAllRequestFile(i);
      }

      // console.log("All paths! This is Kuly new Check here");
      // all_paths.forEach(function(entry) {
      //   console.log(entry);
      // });

      if($scope.archSelect.localeCompare("2tier_ngx8_memc2") == 0){
        throughput_map = throughput_map_2_tier_ngx_8;
        delay_map = delay_map_2_tier_ngx_8;
      }else if($scope.archSelect.localeCompare("social_network") == 0){
        throughput_map = throughput_map_social_network;
        delay_map = delay_map_social_network;
      }



      // Connect to server
      const { EventHubConsumerClient } = require("@azure/event-hubs");
      const connectionString = 'Endpoint=sb://iothub-ns-wzziothub2-3343318-13cac1cedb.servicebus.windows.net/;SharedAccessKeyName=iothubowner;SharedAccessKey=oA3BLe7khEnEhBu9zSsBRpR5fF9aA6GShTtY7/vEbGI=;EntityPath=wzziothub2';
      
      var printError = function (err) {
        console.log(err.message);
      };

      var printMessages = function (messages) {
        for (const message of messages) {
          console.log("Telemetry received: ");
          console.log(JSON.stringify(message.body));
        }
      };

      async function main() {
        console.log("IoT Hub Quickstarts - Read device to cloud messages.");
      
        // If using websockets, uncomment the webSocketOptions below
        // If using proxy, then set `webSocketConstructorOptions` to { agent: proxyAgent }
        // You can also use the `retryOptions` in the client options to configure the retry policy
        const clientOptions = {
          // webSocketOptions: {
          //   webSocket: WebSocket,
          //   webSocketConstructorOptions: {}
          // }
        };
      
        // Create the client to connect to the default consumer group of the Event Hub
        const consumerClient = new EventHubConsumerClient("$Default", connectionString, clientOptions);
      
        // Subscribe to messages from all partitions as below
        // To subscribe to messages from a single partition, use the overload of the same method.
        consumerClient.subscribe({
          processEvents: printMessages,
          processError: printError,
        });
      }
      
      main().catch((error) => {
        console.error("Error running sample:", error);
      });



      for( var j = 0; j < throughput_map.values().next().value.length; j++){
        var arr_temp = [];
        arr_temp.length = $scope.dataset.nodes.length;
        arr_temp.fill(0);
        throughput_display.push([...arr_temp]);
        delay_display.push([...arr_temp]);
      }
      // Create throughput and delay vector based on the map
      for (let [key, value] of throughput_map) {
        var index = nodeIndexMap.get(key);
        // console.log(key);
        for(var j = 0; j < value.length; j++){
          throughput_display[j][index] = value[j];
        }
      }
      for (let [key, value] of delay_map) {
        var index = nodeIndexMap.get(key);
        // console.log(key);
        for(var j = 0; j < value.length; j++){
          delay_display[j][index] = value[j];
        }
      }

      console.log(throughput_display);
      console.log(delay_display);

      // Start to put the flow
      // clearTimeOutvar1 = setTimeout(start_to_put_the_flow, 3000);
      // function start_to_put_the_flow(){
      //   console.log(throughput_map.values().next().value.length);
      //   for(var j = 0; j < throughput_map.values().next().value.length; j++){
      //     for(var k = 0; k < all_paths.length; k++) {
      //       $scope.startFlow(all_paths[k], 10000, j, path_line_graph[k]);
      //     }
      //   }
      // }

      // Show information
      console.log("Kuly Check");
      console.log(all_paths);
      request_path_flag = true;
      temp_index = 0;
      var interval = 1000; // ms
      var old_time = Date.now();
      var flag_flow = true;
      clearTimeOutvar = setTimeout(step, interval*5);
      function step() {
          var new_time = Date.now();
          var dt = new_time - old_time; // the drift (positive for overshooting)
          if (dt > interval) {

            if(flag_flow){
              if(temp_index == throughput_display.length - 1){
                flag_flow = false;
              }
              for(var k = 0; k < all_paths.length; k++) {
                $scope.startFlow(all_paths[k], delay_map_social_network.get("client")[temp_index]/100, 0, path_line_graph[k],temp_index);
              }
            }

            for(var j = 0; j < div_array.length; j++){
              if(throughput_display[temp_index][j] == 0){
                div_array[j].html(String($scope.dataset.nodes[j][0]).substr(String($scope.dataset.nodes[j][0]).lastIndexOf(".") + 1));
              }else{
                div_array[j].html(String($scope.dataset.nodes[j][0]).substr(String($scope.dataset.nodes[j][0]).lastIndexOf(".") + 1) + "<br>" + "Throughput: " + throughput_display[temp_index][j]  + " qps" + "<br>" + "Latency: " + delay_display[temp_index][j]/1000000 + "ms");
              }
            }
            if(temp_index <  throughput_display.length - 1){
              temp_index = temp_index + 1;
            }
            old_time = Date.now();
            // console.log("Good");
            clearTimeOutvar = setTimeout(step, interval);
          }else{
            // console.log("bad");
            clearTimeOutvar = setTimeout(step, interval - dt);
          }
      }
    }

    $scope.startFlow = function(path, end_to_end_delay, start_delay, lineGraph, j){
      var startPoint = [path[0].x, path[0].y];
      var marker_colors = d3.scale.category20b();
      // animation multiple marker
      var marker = $scope.edge_container
        .append("circle")
        .attr("r", 2)
        .style("fill", $scope.markColor[j%9])
        .attr("id", "execution_path_marker")
        .attr("transform", "translate(" + startPoint + ")");
      transition();

      function transition() {
        marker
          .transition()
          .duration(end_to_end_delay) // End to end delay
          .delay(start_delay) // Duration between two dots
          .attrTween("transform", translateAlong(lineGraph));
      }
      // console.log(lineGraph);


      function translateAlong(path) {
        var l = path.getTotalLength();
        return function(i) {
          return function(t) {
            if(!isNaN(t)){
              var p = path.getPointAtLength(t * l);
              return "translate(" + p.x + "," + p.y + ")"; //Move marker
            }else{
              return "translate(" + 0 + "," + 0 + ")";
            }
          };
        };
      }
    }

    //given two nodes, create an edge between them, adjust the node sizes accordingly, then add to dataset
    $scope.addedge = function(s, t) {
      document.getElementById("button1").style.borderStyle = "";

      var sn = $scope.findnode(s);
      var tn = $scope.findnode(t);
      if (sn[0] != "") {
        $scope.dataset.nodes[sn[1]].size++;
      }
      if (tn[0] != "") {
        $scope.dataset.nodes[tn[1]].size++;
      }
      $scope.dataset.edges.push({ source: sn[0], target: tn[0] });
    };

    /**End Edge Creation**/

    /** Undo code**/
    //undo only the last single deleted item by removing it from the toberemoved set and setting it back to visible
    $scope.undoLast = function() {
      $scope.undoData["nodes"].forEach(function(n) {
        n.__data__.toberemoved = false;
        n.style["visibility"] = "visible";
      });

      $scope.undoData["edges"].forEach(function(l) {
        l.__data__.toberemoved = false;
        l.style["visibility"] = "visible";
      });

      //for edges, if it is restored adjust the size of the nodes attached to it
      $scope.addSize($scope.undoData["edges"]);
      d3.selectAll("circle")
        .data($scope.dataset.nodes)
        .attr("r", function(d) {
          return Math.sqrt(d.size) * 2;
        });

      $scope.undoData["edges"] = [];
      $scope.undoData["nodes"] = [];
    };

    //only one deleted item can be set for undoing at a time, so on clean actually remove the items set to be deleted
    $scope.cleanUndo = function() {
      $scope.removeFromDataset(
        $scope.undoData["edges"],
        $scope.undoData["nodes"]
      );

      $scope.undoData["nodes"].forEach(function(n) {
        n.remove();
      });

      $scope.undoData["edges"].forEach(function(l) {
        l.remove();
      });

      $scope.undoData["edges"] = [];
      $scope.undoData["nodes"] = [];
    };
    /**End undo code**/

    /**Node and edge deletion**/

    //Both methods are triggered when respective button pressed. the delection logic is present in either buildNodes or buildEdges

    $scope.deleteNodeSelected = function(e) {
      $scope.resetButtons();
      if ($scope.dataset.nodes.length > 0) {
        $scope.nodeDelete = true;
        document.getElementById(e.currentTarget.id).style.borderStyle = "inset";
      }
    };

    $scope.deleteEdgeSelected = function(e) {
      $scope.resetButtons();
      if ($scope.dataset.edges.length > 0) {
        $scope.edgeDelete = true;
        document.getElementById(e.currentTarget.id).style.borderStyle = "inset";
      }
    };

    $scope.removeLinks = function(nodeName) {
      let sources = $scope.links[0].filter(function(l) {
        return l.__data__.source[0] === nodeName;
      });
      let targets = $scope.links[0].filter(function(l) {
        return l.__data__.target[0] === nodeName;
      });

      var edgesToRemove = [].concat(sources, targets);
      edgesToRemove.forEach(function(l) {
        l.style["visibility"] = "hidden";
        l.__data__.toberemoved = true;
      });
      return edgesToRemove;
    };

    /**End node and edge deletion**/

    /**Transform graph into bundle graph**/
    $scope.bundleGraphSelected = function() {
      $scope.resetButtons();

      window.location.href =
        "index_bundle.html?input=templated_" +
        $scope.archSelect +
        "_" +
        $scope.archstep +
        ".json";
    };
    /**End bundle graph**/

    /**Transform graph into sankey graph**/
    $scope.sankeyGraphSelected = function() {
      $scope.resetButtons();
      window.location.href = "index_sankey.html";
    };
    /**End sankey graph**/

    /**Transform graph into gantt graph**/
    $scope.ganttGraphSelected = function() {
      if ($scope.path.length > 0) {
        $scope.generateGanttJSON();
        $scope.resetButtons();
        window.location.href = "index_gantt.html";
      }
    };

    $scope.generateGanttJSON = function() {
      // generate task list
      var tasks = [];
      var task_names = [];

      // populate tasks from execution path
      var start_time = new Date();
      for (var i = 0; i < $scope.path.length; i++) {
        //console.log($scope.path[i]);
        var node_delay = parseInt(
          $scope.node_data[$scope.path[i][0]].delay,
          10
        );
        var end_time = new Date(start_time.getTime() + node_delay);
        tasks.push({
          startDate: start_time,
          endDate: end_time,
          taskName: $scope.path[i][0],
          status: $scope.node_data[$scope.path[i][0]].package
        });
        task_names.push($scope.path[i][0]);
        start_time = end_time;
      }

      // save to local storage
      let data = JSON.stringify({ tasks, task_names });
      localStorage.setItem("temp_json", data);
    };

    /**End gantt graph**/

    /**Dataset Manipulation**/
    $scope.addnode = function(name) {
      $scope.dataset.nodes.push([name]);
      $scope.dataset.nodes[$scope.nodecount].size = 1;
      $scope.nodecount++;
      return $scope.nodecount - 1;
    };

    $scope.findnode = function(nodename) {
      for (var n = 0; n < $scope.dataset.nodes.length; n++)
        if ($scope.dataset.nodes[n][0] == nodename) {
          return [$scope.dataset.nodes[n], n];
        }
      console.log("can't find " + nodename);
      return ["", -1];
    };

    $scope.removeFromDataset = function(links, nodes) {
      links.forEach(function(l) {
        var found = -1;
        for (var i = 0; i < $scope.dataset.edges.length; i++) {
          var cur = $scope.dataset.edges[i];
          if (
            cur.source[0] == l.__data__.source[0] &&
            cur.target[0] == l.__data__.target[0]
          ) {
            found = i;
          }
        }

        if (found > -1) {
          var r = $scope.dataset.edges.splice(found, 1);
        }
      });

      nodes.forEach(function(n) {
        var sn = $scope.findnode(n.__data__[0]);
        if (sn[1] > -1) {
          $scope.dataset.nodes.splice(sn[1], 1);
        }
      });
    };
    /**End dataset manipulation**/

    /**Enabling and disabling zoom**/

    //zoom object for adjusting the button text according to zoom state
    $scope.zoomObj = {};
    $scope.zoomObj["state"] = 0;
    $scope.zoomObj["buttonText"] = "Disable Zoom";

    $scope.removeZoom = function() {
      if ($scope.zoomObj["state"] == 0) {
        zoom.on("zoom", null);
        $scope.zoomObj["buttonText"] = "Enable Zoom";
        $scope.zoomObj["state"] = 1;
      } else {
        zoom.on("zoom", zoomed);
        $scope.zoomObj["buttonText"] = "Disable Zoom";
        $scope.zoomObj["state"] = 0;
      }
    };
    /**End zoom disable and enable**/

    /**Help button**/
    $scope.help = function() {
      var data =
        "<b>Create Edge</b> <br> Click on a start node and then click on an end node. " +
        " The arrow that appears is to show where the edge will point. <br> <br>" +
        " <b>Create Node</b> <br> Fill in all parts of the form, then submit it. Click on the page where you want the new node to be. <br><br>" +
        " <b>Delete Edge</b> <br> Click on delete edge and then click on edge you want to delete. <br><br>" +
        " <b>Delete Node</b> <br> Click on delete node and then click on node you want to delete. <br><br>" +
        " <b>Undo Last Delete</b> <br> Undo the last delete edge or delete node. If no deletes occurred, nothing will happen. <br><br>" +
        " <b>Cancel Action</b> <br> If any action button (i.e. create node, delete edge) is selected, remove state of being in this action mode <br><br>" +
        " <b>Bundle Graph</b> <br> Creates a graph showing the connections between each node on current graph. <br><br>" +
        " <b>Network Graph</b> <br> Creates a graph showing the connectivity/network flow in the dependency graph. <br><br>" +
        " <b>Gantt Graph</b> <br> Creates a Gantt graph showing the latency breakdown in the dependency graph. <br><br>" +
        " <b>Show Export</b> <br> Shows the entire graph as a JSON, including any additions or deletions made. <br><br>" +
        " <b>Disable Zoom</b> <br> Temporarily stops zooming function. Can be re-enabled.";
      $scope.myWindow = window.open(
        "data:text/html," + encodeURIComponent(data),
        "_blank",
        "width=500, height=200"
      );
      $scope.myWindow.focus();
    };
    /**End help button**/

    /**Util methods**/
    $scope.getDependencies = function(nodeName) {
      return $scope.links[0].filter(function(l) {
        return l.__data__.source[0] === nodeName;
      });
    };

    function neighboring(a, b) {
      return $scope.linked[a.index + "," + b.index];
    }

    $scope.removeNum = function(s) {
      return s.replace(/[0-9]/g, "");
    };

    //resets buttons such that none are pressed and empties the new node form. zoom is renabled
    $scope.resetButtons = function() {
      for (var i = 1; i < 5; i++) {
        document.getElementById("button" + i).style.borderStyle = "";
      }

      $scope.nodeDelete = false;
      $scope.nodeCreate = false;
      $scope.edgeCreate = false;
      $scope.edgeDelete = false;

      $scope.startNode = null;
      $scope.endNode = null;

      var defaultForm = {
        name: undefined,
        arch: undefined,
        region: undefined,
        zone: undefined,
        package: undefined,
        delay: undefined
      };

      $scope.node = angular.copy(defaultForm);

      document.getElementById("nodeFormContainer").style.display = "none";

      if ($scope.zoomObj["state"] != 0) {
        zoom.on("zoom", zoomed);
        $scope.zoomObj["buttonText"] = "Disable Zoom";
        $scope.zoomObj["state"] = 0;
      }
    };

    /**Utility methods for adjusting node size according to edges**/
    $scope.subtractSize = function(edges) {
      edges.forEach(function(l) {
        var s = l.__data__.source[0];
        var t = l.__data__.target[0];

        var sn = $scope.findnode(s);
        var tn = $scope.findnode(t);
        if (sn[0] != "") {
          if ($scope.dataset.nodes[sn[1]].size > 0) {
            $scope.dataset.nodes[sn[1]].size--;
          }
        }
        if (tn[0] != "") {
          if ($scope.dataset.nodes[tn[1]].size > 0) {
            $scope.dataset.nodes[tn[1]].size--;
          }
        }
      });
    };

    $scope.addSize = function(edges) {
      edges.forEach(function(l) {
        var s = l.__data__.source[0];
        var t = l.__data__.target[0];

        var sn = $scope.findnode(s);
        var tn = $scope.findnode(t);
        if (sn[0] != "") {
          $scope.dataset.nodes[sn[1]].size++;
        }
        if (tn[0] != "") {
          $scope.dataset.nodes[tn[1]].size++;
        }
      });
    };
    /**End node size utility methods**/

    // gantt chart method
    function makeGant(tasks, pageWidth, pageHeight) {
      var barHeight = 20;
      var gap = barHeight + 4;
      var topPadding = 75;
      var sidePadding = 75;

      var colorScale = d3
        .scaleLinear()
        .domain([0, categories.length])
        .range(["#00B9FA", "#F95002"])
        .interpolate(d3.interpolateHcl);

      makeGrid(sidePadding, topPadding, pageWidth, pageHeight);
      drawRects(
        tasks,
        gap,
        topPadding,
        sidePadding,
        barHeight,
        colorScale,
        pageWidth,
        pageHeight
      );
      vertLabels(gap, topPadding, sidePadding, barHeight, colorScale);
    }

    function drawRects(
      theArray,
      theGap,
      theTopPad,
      theSidePad,
      theBarHeight,
      theColorScale,
      w,
      h
    ) {
      var bigRects = svg
        .append("g")
        .selectAll("rect")
        .data(theArray)
        .enter()
        .append("rect")
        .attr("x", 0)
        .attr("y", function(d, i) {
          return i * theGap + theTopPad - 2;
        })
        .attr("width", function(d) {
          return w - theSidePad / 2;
        })
        .attr("height", theGap)
        .attr("stroke", "none")
        .attr("fill", function(d) {
          for (var i = 0; i < categories.length; i++) {
            if (d.type == categories[i]) {
              return d3.rgb(theColorScale(i));
            }
          }
        })
        .attr("opacity", 0.2);

      var rectangles = svg
        .append("g")
        .selectAll("rect")
        .data(theArray)
        .enter();

      var innerRects = rectangles
        .append("rect")
        .attr("rx", 3)
        .attr("ry", 3)
        .attr("x", function(d) {
          return timeScale(dateFormat(d.startTime)) + theSidePad;
        })
        .attr("y", function(d, i) {
          return i * theGap + theTopPad;
        })
        .attr("width", function(d) {
          return (
            timeScale(dateFormat(d.endTime)) -
            timeScale(dateFormat(d.startTime))
          );
        })
        .attr("height", theBarHeight)
        .attr("stroke", "none")
        .attr("fill", function(d) {
          for (var i = 0; i < categories.length; i++) {
            if (d.type == categories[i]) {
              return d3.rgb(theColorScale(i));
            }
          }
        });

      var rectText = rectangles
        .append("text")
        .text(function(d) {
          return d.task;
        })
        .attr("x", function(d) {
          return (
            (timeScale(dateFormat(d.endTime)) -
              timeScale(dateFormat(d.startTime))) /
              2 +
            timeScale(dateFormat(d.startTime)) +
            theSidePad
          );
        })
        .attr("y", function(d, i) {
          return i * theGap + 14 + theTopPad;
        })
        .attr("font-size", 11)
        .attr("text-anchor", "middle")
        .attr("text-height", theBarHeight)
        .attr("fill", "#fff");

      rectText
        .on("mouseover", function(e) {
          // console.log(this.x.animVal.getItem(this));
          var tag = "";

          if (d3.select(this).data()[0].details != undefined) {
            tag =
              "Task: " +
              d3.select(this).data()[0].task +
              "<br/>" +
              "Type: " +
              d3.select(this).data()[0].type +
              "<br/>" +
              "Starts: " +
              d3.select(this).data()[0].startTime +
              "<br/>" +
              "Ends: " +
              d3.select(this).data()[0].endTime +
              "<br/>" +
              "Details: " +
              d3.select(this).data()[0].details;
          } else {
            tag =
              "Task: " +
              d3.select(this).data()[0].task +
              "<br/>" +
              "Type: " +
              d3.select(this).data()[0].type +
              "<br/>" +
              "Starts: " +
              d3.select(this).data()[0].startTime +
              "<br/>" +
              "Ends: " +
              d3.select(this).data()[0].endTime;
          }
          var output = document.getElementById("tag");

          var x = this.x.animVal.getItem(this) + "px";
          var y = this.y.animVal.getItem(this) + 25 + "px";

          output.innerHTML = tag;
          output.style.top = y;
          output.style.left = x;
          output.style.display = "block";
        })
        .on("mouseout", function() {
          var output = document.getElementById("tag");
          output.style.display = "none";
        });

      innerRects
        .on("mouseover", function(e) {
          //console.log(this);
          var tag = "";

          if (d3.select(this).data()[0].details != undefined) {
            tag =
              "Task: " +
              d3.select(this).data()[0].task +
              "<br/>" +
              "Type: " +
              d3.select(this).data()[0].type +
              "<br/>" +
              "Starts: " +
              d3.select(this).data()[0].startTime +
              "<br/>" +
              "Ends: " +
              d3.select(this).data()[0].endTime +
              "<br/>" +
              "Details: " +
              d3.select(this).data()[0].details;
          } else {
            tag =
              "Task: " +
              d3.select(this).data()[0].task +
              "<br/>" +
              "Type: " +
              d3.select(this).data()[0].type +
              "<br/>" +
              "Starts: " +
              d3.select(this).data()[0].startTime +
              "<br/>" +
              "Ends: " +
              d3.select(this).data()[0].endTime;
          }
          var output = document.getElementById("tag");

          var x = this.x.animVal.value + this.width.animVal.value / 2 + "px";
          var y = this.y.animVal.value + 25 + "px";

          output.innerHTML = tag;
          output.style.top = y;
          output.style.left = x;
          output.style.display = "block";
        })
        .on("mouseout", function() {
          var output = document.getElementById("tag");
          output.style.display = "none";
        });
    }

    function makeGrid(theSidePad, theTopPad, w, h) {
      var xAxis = d3
        .axisBottom(timeScale)
        .ticks(d3.timeDay, 1)
        .tickSize(-h + theTopPad + 20, 0, 0)
        .tickFormat(d3.timeFormat("%d %b"));

      var grid = svg
        .append("g")
        .attr("class", "grid")
        .attr("transform", "translate(" + theSidePad + ", " + (h - 50) + ")")
        .call(xAxis)
        .selectAll("text")
        .style("text-anchor", "middle")
        .attr("fill", "#000")
        .attr("stroke", "none")
        .attr("font-size", 10)
        .attr("dy", "1em");
    }

    function vertLabels(
      theGap,
      theTopPad,
      theSidePad,
      theBarHeight,
      theColorScale
    ) {
      var numOccurances = new Array();
      var prevGap = 0;

      for (var i = 0; i < categories.length; i++) {
        numOccurances[i] = [
          categories[i],
          getCount(categories[i], catsUnfiltered)
        ];
      }

      var axisText = svg
        .append("g") //without doing this, impossible to put grid lines behind text
        .selectAll("text")
        .data(numOccurances)
        .enter()
        .append("text")
        .text(function(d) {
          return d[0];
        })
        .attr("x", 10)
        .attr("y", function(d, i) {
          if (i > 0) {
            for (var j = 0; j < i; j++) {
              prevGap += numOccurances[i - 1][1];
              // console.log(prevGap);
              return (d[1] * theGap) / 2 + prevGap * theGap + theTopPad;
            }
          } else {
            return (d[1] * theGap) / 2 + theTopPad;
          }
        })
        .attr("font-size", 11)
        .attr("text-anchor", "start")
        .attr("text-height", 14)
        .attr("fill", function(d) {
          for (var i = 0; i < categories.length; i++) {
            if (d[0] == categories[i]) {
              //  console.log("true!");
              return d3.rgb(theColorScale(i)).darker();
            }
          }
        });
    }

    //from this stackexchange question: http://stackoverflow.com/questions/1890203/unique-for-arrays-in-javascript
    function checkUnique(arr) {
      var hash = {},
        result = [];
      for (var i = 0, l = arr.length; i < l; ++i) {
        if (!hash.hasOwnProperty(arr[i])) {
          //it works with objects! in FF, at least
          hash[arr[i]] = true;
          result.push(arr[i]);
        }
      }
      return result;
    }

    //from this stackexchange question: http://stackoverflow.com/questions/14227981/count-how-many-strings-in-an-array-have-duplicates-in-the-same-array
    function getCounts(arr) {
      var i = arr.length, // var to loop over
        obj = {}; // obj to store results
      while (i) obj[arr[--i]] = (obj[arr[i]] || 0) + 1; // count occurrences
      return obj;
    }

    // get specific from everything
    function getCount(word, arr) {
      return getCounts(arr)[word] || 0;
    }

    // save text
    function saveText(text, filename) {
      var a = document.createElement("a");
      a.setAttribute(
        "href",
        "data:text/plain;charset=utf-u," + encodeURIComponent(text)
      );
      console.log(filename);
      a.setAttribute("download", filename);
      a.click();
    }

    /**End Utility Methods**/
  });
