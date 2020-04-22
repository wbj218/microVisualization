'use strict';


angular.module('myApp.home', ['ngRoute'])

.config(['$routeProvider', function($routeProvider) {
	$routeProvider.when('/home', {
		templateUrl: 'home/home.html',
		controller: 'homeCtrl'
	});
}])

.controller('homeCtrl', function($scope, $http) {
	/** Defines and Constants */
	// Holds data about which nodes and edges are selected
	$scope.selectednodes = [];
	$scope.selectededges = [];

	// Constant d3 charge
	$scope.charge = -200;

	// Number of nodes
	$scope.nodecount = 0;

	// Graph data
	$scope.dataset = {
		nodes: [],
		edges: []
	};
	$scope.node_data = {};

	// Execution Path Data
	$scope.path = [];
	$scope.node_dict = {};

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
	$scope.colors = d3.scaleOrdinal(d3.schemeCatergory10);
	$scope.next_color = 0;
	$scope.color_dict = {};
	/** END Defines and Constants */

	/** Architecture change code */
	d3.json("./inputs/arch_directory.json", function(err,json){
		console.log(json);
		$scope.architectures = json["arch"];
	});

	// read request collection json file
	d3.json("./inputs/test_paths/requests.json", function(err, json) {
		console.log(json);
		$scope.requests = json["requests"];
	});

	// parse request json file and display execution path
	$scope.loadRequestFile = function(err, json) {
		console.log(json);
		
		var i;
		
		for (i = 0; i < $scope.dataset.nodes.length; i++) {
			$scope.node_dict[$scope.dataset.nodes[i][0]] = $scope.dataset.nodes[i];
		}
		
		for (i = 0; i < json["path"].length; i++) {
			$scope.path.push($scope.node_dict[json["path"][i]]);
		}
		console.log($scope.path);
		$scope.displayExecutionPath();
	};

	$scope.displayExecutionPath = function() {
		$scope.edge_container.select("#execution_path").remove();
		$scope.edge_container.select("#execution_path_marker").remove();
		var lineFunction = d3.svg.line()
						.x(function(d) { return d.x; })
						.y(function(d) { return d.y; })
						.interpolate("linear");
		var lineGraph = $scope.edge_container.append("path")
						.attr("d", lineFunction($scope.path))
						.attr("stroke", "red")
						.attr("stroke-width", 2)
						.attr("id", "execution_path")
						.attr("fill", "none");

		var startPoint = [$scope.path[0].x, $scope.path[0].y];
		var marker = $scope.edge_container.append("circle")
					.attr("r", 4)
					.style("fill", "green")
					.attr("id", "execution_path_marker")
					.attr("transform", "translate(" + startPoint + ")");
  		transition();
		function transition() {
			marker.transition()
				.duration(6000)
				.attrTween("transform", translateAlong(lineGraph.node()))
				.each("end", transition);// infinite loop
		}
		function translateAlong(path) {
			var l = path.getTotalLength();
			return function(i) {
			  return function(t) {
				var p = path.getPointAtLength(t * l);
				return "translate(" + p.x + "," + p.y + ")";//Move marker
			  }
			}
		}
	}

	$scope.tick = function() {
		$scope.edge_container.selectAll("line").attr("x1", function(d) { return d.source.x; })
			 .attr("y1", function(d) { return d.source.y; })
			 .attr("x2", function(d) { return d.target.x; })
			 .attr("y2", function(d) { return d.target.y; });
		$scope.node_container.selectAll("circle").attr("cx", function(d) { return d.x; })
			 .attr("cy", function(d) { return d.y; });
	};

	$scope.fileLoad = function(err,json) {
		console.log(json);
		$scope.json = json;
		$scope.edge_container.selectAll("*").remove();
		$scope.node_container.selectAll("*").remove();
		$scope.dataset.nodes = [];
		$scope.dataset.edges = [];
		$scope.nodecount = 0;

		for (var i = 0; i < $scope.json.graph.length; i++) {
			var element = $scope.json.graph[i];
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
		$scope.force = d3.forceSimulation()
            .force("link", d3.forceLink().id(function(d,i) {
                return i;
            }).distance(50))
            .force("charge", d3.forceManyBody().strength(-100 ))
            .force("center", d3.forceCenter(svgWidth / 2,svgHeight / 2));
        
        simulation
            .nodes($scope.dataset.nodes)
            .on("tick", ticked);
    	simulation
            .force("link")
            .links($scope.dataset.edges);
		// $scope.force = d3.forceSimulation()
		// 	.nodes($scope.dataset.nodes)
		// 	.links($scope.dataset.edges)
		// 	.size([w, h])
		// 	.charge([$scope.charge])
		// 	.linkDistance(function(d) {
		// 		return 10 + 7 * (d.source.size + d.target.size)/2;
		// 	});

		$scope.force.on("tick", $scope.tick);

		$scope.update();
		$scope.force.start();
		for (var i = 1000; i > 0; --i) $scope.force.tick();
		$scope.force.start();
	    //$scope.force.stop();
	};

	$scope.changeArch = function(newArch) {
		$scope.archstep = 0;
		d3.json("./inputs/"+$scope.archSelect+"_"+$scope.archstep+".json", $scope.fileLoad);
	};

	// depict new execution path
	$scope.changeExecutionPath = function(newRequest) {
		d3.json("./inputs/test_paths/"+$scope.requestSelect+".json", $scope.loadRequestFile);
	};

	$scope.prevStep = function() {
		if ($scope.archstep > 0) {
			$scope.archstep = $scope.archstep - 1;
			d3.json("./inputs/"+$scope.archSelect+"_"+$scope.archstep+".json", $scope.fileLoad);
		}
	};

	$scope.nextStep = function() {
		$scope.archstep = $scope.archstep + 1;
		d3.json("./inputs/"+$scope.archSelect+"_"+$scope.archstep+".json", function(err,json) {
			if (json) {
				$scope.fileLoad(err,json);
			} else {
				$scope.archstep = $scope.archstep - 1;
			}
		});
	};
	/** END architecture change code */

	/** Export Code */
	$scope.exportArch = function() {
		if ($scope.exportObj["state"] == 1) {
			$scope.myWindow.close();
			$scope.exportObj["state"] = 0;
			$scope.exportObj["buttonText"] = "Show Export";
			return;
		}
		var result = {};
		result["arch"] = $scope.archSelect;
		result["step"] = $scope.archstep;
		result["graph"] = [];

		var edgecount = 1;
		$scope.dataset.edges.forEach(function(l){
			if (!l.toberemoved) {
				var edgeobj = {};
				edgeobj["source"] = l.source[0];
				edgeobj["target"] = l.target[0];
				edgeobj["edge"] = "e"+edgecount;
				edgecount = edgecount + 1;
				result["graph"].push(edgeobj);
			}
		});

		$scope.dataset.nodes.forEach(function(n) {
			if (!n.toberemoved) {
				var nodeobj = {};
				nodeobj["node"] = n[0];
				result["graph"].push(nodeobj);
			}
		});

		$scope.exportObj['json'] = JSON.stringify(result);
		$scope.exportObj["state"] = 1;
		$scope.exportObj["buttonText"] = "Hide Export";
		//opens the export json in a new window, but only works if there is no popup blocker...
		var data = $scope.exportObj['json'];
		$scope.myWindow = window.open("data:text/html," + encodeURIComponent(data), "_blank", "width=500, height=200");
		$scope.myWindow.focus();
		console.log(JSON.stringify(result));
	}
	/** END Export Code */

	/** Defines Visualization container and container interactions */
	$scope.trans = [0,0];
	$scope.scale = 0;

	//if user has scrolled, this returns the actual x, y on screen given translation and scaling matrices
	function getScreenCoords(x, y, translate, scale) {
		var xn = translate[0] + x*scale;
		var yn = translate[1] + y*scale;
		return { x: xn, y: yn };
	}

	/**Zooming in and out**/
	//lets user zoom in and out only if is not in edge create mode
	function zoomed() {
		if ($scope.edgeCreate) return;
		$scope.hasZoomed = true;
		$scope.edge_container.attr("transform", "translate(" + d3.event.translate + ")scale(" + d3.event.scale + ")");
		$scope.node_container.attr("transform", "translate(" + d3.event.translate + ")scale(" + d3.event.scale + ")");
		$scope.trans = d3.event.translate;
		$scope.scale = d3.event.scale;
	}

	var zoom = d3.zoom()
		.scaleExtent([.1, 30])
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

	$scope.drag = d3.drag()
		.subject(function(d) { return d; })
		.on("start", dragstarted)
		.on("drag", dragged)
		.on("end", dragended);

	/**End drag section**/

	// SVG
	$scope.svg = d3.select("body")
				.append("svg")
				.attr("width", w)
				.attr("height", h)
				.append("g")
				.call(zoom)
				.on("mousedown.zoom", null)
				.on("dblclick.zoom", null)

	$scope.svg.append('svg:defs')
		.append('svg:marker')
			.attr('id', 'end-arrow')
			.attr('viewBox', '0 -5 10 10')
			.attr('refX', 6)
			.attr('markerWidth', 3)
			.attr('markerHeight', 3)
			.attr('orient', 'auto')
		.append('svg:path')
			.attr('d', 'M0,-5L10,0L0,5')
			.attr('fill', '#000');

	$scope.svg.append("svg:defs")
		.append("svg:marker")
			.attr("id", "triangle")
			.attr("refX", 6)
			.attr("refY", 6)
			.attr("markerWidth", 30)
			.attr("markerHeight", 30)
			.attr("markerUnits","userSpaceOnUse")
			.attr("orient", "auto")
		.append("path")
			.attr("d", "M 0 0 12 6 0 12 3 6")
			.style("fill", "black");

	$scope.svg.append("svg:defs")
		.append("svg:marker")
			.attr("id", "arrow")
			.attr("viewBox", "0 -5 10 10")
			.attr('refX', -20)
			.attr("markerWidth", 5)
			.attr("markerHeight", 5)
			.attr("orient", "auto")
		.append("svg:path")
			.attr("d", "M0,-5L10,0L0,5");

	var rect = $scope.svg.append("rect")
		.attr("width", w)
		.attr("height", h)
		.style("fill", "none")
		.style("pointer-events", "all")
		//mousemove only relevant for edge creation, so if there is no startNode for beginning of edge set, return
		.on("mousemove", function(){
			if(!$scope.startNode) return;
			var coords = ($scope.hasZoomed ?
				getScreenCoords($scope.startNode.x, $scope.startNode.y, $scope.trans, $scope.scale) :
				{"x" : $scope.startNode.x, "y" : $scope.startNode.y});

			 // update drag line
			$scope.drag_line.attr('d', 'M' + coords.x + ',' + coords.y + 'L' + d3.mouse(this)[0] + ',' + d3.mouse(this)[1]);
		})
		//clicking creates a node at that point if in nodeCreate mode, otherwise can drag the graph around
		.on("click", function(s){
			if ($scope.nodeCreate){
				$scope.createNode(d3.mouse(this));
			}

			d3.selectAll("circle").transition()
			.duration(300)
			.style('opacity', function(n) {
				return 1;
			});

			d3.selectAll("line").transition()
			.duration(300)
			.style('opacity', function(l) {
				return 1;
			});
		});

	$scope.edge_container = $scope.svg.append("g");
	$scope.node_container = $scope.svg.append("g");
	//drag_line that appears when creating an edge to show what 2 nodes you are linking
	$scope.drag_line = $scope.svg.append('svg:path')
			.attr('class', 'link dragline hidden')
			.attr('d', 'M0,0L0,0')
			.attr("stroke-width", 2)
			.attr("stroke", "black");

	//defines the edges in the graph
	$scope.buildEdges = function() {
		return $scope.edge_container.selectAll("line")
			.data($scope.dataset.edges)
			.enter()
			.append("line")
			.style("stroke", function(d) {
				return "#ccc";
			})
			.style("stroke-width", 2)
			.attr("x1", function(d){
				return d.source.x;
			})
			.attr('marker-end', function(d) {
				return "url(#triangle)";
			})
			.attr("y1", function(d){
				return d.source.y;
			})
	 		.attr("x2", function(d){
				return d.target.x;
			})
			.attr("y2", function(d){
				return d.target.y;
			})
			//click is only relevant if in edge deletion mode
			.on("click", function(d){
				if($scope.edgeDelete){
					$scope.cleanUndo(); //can only undo 1 delete at a time, so clean up all existing to be deleted objects
					this.style['visibility'] = "hidden"; //hide it in case user wants to cancel delete
					this.__data__.toberemoved = true; //sets it to be removed
					$scope.undoData['nodes'] = [];
					$scope.undoData['edges'] = [this];
					$scope.subtractSize([this]); //nodes connected to it need to be made smaller
					d3.selectAll("circle").data($scope.dataset.nodes).attr("r", function(d){ return Math.sqrt(d.size) * 2; });
					$scope.edgeDelete = false;
					document.getElementById('button3').style.borderStyle = "";
				}
			});
	}


	//poly = [{"x":0.0, "y":25.0},
	//     {"x":8.5,"y":23.4},
	//     {"x":13.0,"y":21.0},
	//     {"x":19.0,"y":15.5}];

		//return $scope.node_container.selectAll("polygon")
		//	.data([poly])
		//	.enter().append("polygon")
		//	.attr("points", function(d) {
		//		return d.map(function(d) {
		//			return [scaleX(d.x), scale(d.y)].join(",");
		//		}).join(" ");
		//	})
		//	.attr("stroke","black")
		//	.attr("stroke-width",2)
	//nodes in the graph
	$scope.buildNodes = function() {
		return $scope.node_container.selectAll("circle")
			.data($scope.dataset.nodes)
			.enter()
			.append("circle")
			.attr("r", function(d, i) {
			return Math.sqrt(d.size) * 2;
			/**return Math.sqrt(d.size) * 2.4;**/
			})
			//fill is based on the application, zone, name such that they are grouped by color
			.style("fill", function(d, i) {
				var names = d[0].split(".")
				//TODO: changed to 3 because of twitter format, change back to 4 for spigo graphs
				//if (names.length < 3) {
				if (names.length < 4) {
					return $scope.colors(0)
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
							return $scope.colors(shade)
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
						}
						else {
							$scope.colors = d3.scale.category20c();
							$scope.next_color = -4
							$scope.next_color = ($scope.next_color + 4) % 20;
						}
						return $scope.colors(colorObj["mapping"][zone]);
					}
				}
			})
			.style("stroke", "black")
			.style("stroke-width", 1)
			.on("click", function(d){
				//like in edge delete, we need to ensure only one thing can be undo'd at a time
				//we save the node until we are sure the user will not undo it
				if($scope.nodeDelete){
					$scope.cleanUndo();

					var toRemove = $scope.removeLinks(d[0]);
					$scope.subtractSize(toRemove);
					d3.selectAll("circle").data($scope.dataset.nodes).attr("r", function(d){ return Math.sqrt(d.size) * 2; });
					$scope.undoData['edges'] = toRemove;
					$scope.undoData['nodes'] = [this];
					this.style['visibility'] = "hidden";
					this.__data__.toberemoved = true;
					$scope.nodeDelete = false;
					document.getElementById('button4').style.borderStyle = "";
					d3.select("#tooltip").classed("hidden", true);
				} else if($scope.edgeCreate){
					if(!$scope.startNode){
						$scope.startNode = d;
						//in case user has zoomed, need to get real coordinates of the point based on transformation
						var coords = ($scope.hasZoomed ? getScreenCoords(d.x, d.y, $scope.trans, $scope.scale) : {"x" : d.x, "y" : d.y});

						// reposition drag line
						$scope.drag_line
							.style('marker-end', 'url(#end-arrow)')
							.classed('hidden', false)
							 .attr('d', 'M' + coords.x + ',' + coords.y + 'L' + coords.x + ',' + coords.y);
					} else{
						//if there is already a start node, connect the drag_line to the end node only if start node != end node

						//hide the drag_line when a real edge is made
						$scope.drag_line
							.classed('hidden', true)
							.style('marker-end', '');

						// check for drag-to-self
						$scope.endNode = d;
						if($scope.startNode === $scope.endNode) {  return; }
						$scope.addedge($scope.startNode[0], $scope.endNode[0]);
						$scope.buildEdges();
						$scope.links = $scope.edge_container.selectAll("line");
						$scope.resetButtons();
					}
				}
				else{
					//highlight the node and all edges connected to it
					$scope.linked = {};
					for (var i = 0; i < $scope.force.nodes().length; i++) {
						$scope.linked[$scope.force.nodes()[i].index+","+$scope.force.nodes()[i].index] = 1;
					}
					for (var j = 0; j < $scope.force.links().length; j++) {
						$scope.linked[$scope.force.links()[j].source.index+","+$scope.force.links()[j].target.index] = 1;
					}
					const cur = d3.select(this).node().__data__;
					d3.selectAll("circle").transition()
						.duration(100)
						.style('opacity', (o) => {
							return neighboring(cur, o) | neighboring(o, cur) ? 1 : 0.2;
						});

					d3.selectAll("line").transition()
						.duration(100)
						.style('opacity', function (o) {
							return cur.index === o.source.index | cur.index === o.target.index ? 1 : 0.2;
						});
				}
			})
			.on("mouseover", function(d) {
				//Get this node's x/y values, then augment for the tooltip
				var dependencies = $scope.getDependencies(d[0]);
				var dependList = "";
				dependencies.forEach(function(e){
					dependList += e.__data__.target[0] + "<br>";
				});
				var xPosition = w-500;
				var yPosition = 60;
				//Update the tooltip position and value
				d3.select("#tooltip")
					.style("left", xPosition + "px")
					.style("top", yPosition + "px")
					.select("#value")
					.style("z-value", 9999)
					.html("Name: " + d + "<br>" +
						"Dependencies: " + "<br>"  + dependList);
				//Show the tooltip
				d3.select("#tooltip").classed("hidden", false);
				d3.event.stopPropagation();
			})
			.on("mouseleave", function() {
				//Hide the tooltip
				d3.select("#tooltip").classed("hidden", true);
			})
			.call($scope.drag)
			.attr("cx", function(d){ return d.x; })
			.attr("cy", function(d) { return d.y; });
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
	$scope.createNodeSelected = function(e){
		$scope.resetButtons();
		$scope.nodeCreate = true;
		document.getElementById(e.currentTarget.id).style.borderStyle = "inset";
		document.getElementById("nodeFormContainer").style.display = "block";
	}

	//upon node creation form submission trigger this
	$scope.newNode = function(){
		var form = document.getElementById("nodeFormContainer");
		var error = document.getElementById("nodeFormError");
		//check that all fields of form are filled in
		if (typeof $scope.node === "undefined" || typeof $scope.node.name === "undefined" || typeof $scope.node.arch === "undefined" ||
			typeof $scope.node.region === "undefined" || typeof $scope.node.zone == "undefined"){
			//display an error if not all filled
			error.style.display = "block";
		} else{
			form.style.display = "none";
			error.style.display = "none";
			//create name by simple concatenation
			var name = $scope.node.name + "." + $scope.node.arch + "." + $scope.node.region + "." + $scope.node.zone;
			var n = $scope.addnode(name);

			//clear the form
			var defaultForm = {
				name: undefined,
				arch: undefined,
				region: undefined,
				zone: undefined
			}

			$scope.node = angular.copy(defaultForm);
			document.getElementById('button2').style.borderStyle = "";
		}
	}

	//after form has been submitted, this method lets users create a node at where they click
	//accounts for zoom by transforming the coordinates if user has zoomed
	$scope.createNode = function(point){
		var n = $scope.dataset.nodes.length - 1;
		$scope.dataset.nodes[n].x = $scope.hasZoomed ? (point[0] -  $scope.trans[0]) / $scope.scale : point[0];
		$scope.dataset.nodes[n].y = $scope.hasZoomed ? (point[1] - $scope.trans[1]) / $scope.scale : point[1];
		$scope.buildNodes();
		$scope.nodes = $scope.node_container.selectAll("circle");
		$scope.resetButtons();
	}

	/**End Node Creation**/

	/**Edge Creation**/
	//method triggered when user selects create edge button. Note that the rest of edge creation logic is in on click for nodes
	//once mode is set to edge creation, if user clicks on a node the drag_line will appear. clicking on second different node
	//will result in an edge

	$scope.createEdgeSelected = function(e){
		$scope.resetButtons();
		if ($scope.dataset.nodes.length > 1){

			document.getElementById(e.currentTarget.id).style.borderStyle = "inset";

			$scope.edgeCreate = true;
		}
	}

	//given two nodes, create an edge between them, adjust the node sizes accordingly, then add to dataset
	$scope.addedge = function(s, t) {
		document.getElementById('button1').style.borderStyle = "";

		var sn = $scope.findnode(s);
		var tn = $scope.findnode(t);
		if(sn[0] != ""){
			$scope.dataset.nodes[sn[1]].size++;
		}
		if(tn[0] != ""){
			$scope.dataset.nodes[tn[1]].size++;
		}
		$scope.dataset.edges.push({ source: sn[0], target: tn[0] });
	};

	/**End Edge Creation**/

	/** Undo code**/
	//undo only the last single deleted item by removing it from the toberemoved set and setting it back to visible
	$scope.undoLast = function() {
		$scope.undoData['nodes'].forEach(function(n) {
			n.__data__.toberemoved = false;
			n.style['visibility'] = "visible";
		});

		$scope.undoData['edges'].forEach(function(l) {
			l.__data__.toberemoved = false;
			l.style['visibility'] = "visible";
		});

		//for edges, if it is restored adjust the size of the nodes attached to it
		$scope.addSize($scope.undoData['edges']);
		d3.selectAll("circle").data($scope.dataset.nodes).attr("r", function(d){ return Math.sqrt(d.size) * 2; });

		$scope.undoData['edges'] = [];
		$scope.undoData['nodes'] = [];
	}

	//only one deleted item can be set for undoing at a time, so on clean actually remove the items set to be deleted
	$scope.cleanUndo = function() {
		$scope.removeFromDataset($scope.undoData['edges'], $scope.undoData['nodes']);

		$scope.undoData['nodes'].forEach(function(n) {
			n.remove();
		});

		$scope.undoData['edges'].forEach(function(l) {
			l.remove();
		});

		$scope.undoData['edges'] = [];
		$scope.undoData['nodes'] = [];
	}
	/**End undo code**/

	/**Node and edge deletion**/

	//Both methods are triggered when respective button pressed. the delection logic is present in either buildNodes or buildEdges

	$scope.deleteNodeSelected = function(e){
		$scope.resetButtons();
		if ($scope.dataset.nodes.length > 0){
			$scope.nodeDelete = true;
			document.getElementById(e.currentTarget.id).style.borderStyle = "inset";
		}
	};

	$scope.deleteEdgeSelected = function(e){
		$scope.resetButtons();
		if ($scope.dataset.edges.length > 0){
			$scope.edgeDelete = true;
			document.getElementById(e.currentTarget.id).style.borderStyle = "inset";
		}

	};

	$scope.removeLinks = function(nodeName){
		let sources = $scope.links[0].filter(function(l){
			return l.__data__.source[0] === nodeName;
		});
		let targets = $scope.links[0].filter(function(l){
			return l.__data__.target[0] === nodeName;
		});

		var edgesToRemove = [].concat(sources, targets);
		edgesToRemove.forEach(function(l){
			l.style['visibility'] = "hidden";
			l.__data__.toberemoved = true;
		});
		return edgesToRemove;
	}

	/**End node and edge deletion**/

	$scope.packageHierarchy = function(classes) {
		var map = {};

		function find(name, data) {
			var node = map[name], i;
			if (!node) {
				node = map[name] = data || {name: name, children: []};
				if (name.length) {
					node.parent = find(name.substring(0, i = name.lastIndexOf(".")));
					node.parent.children.push(node);
					node.key = name.substring(i + 1);
				}
			}
			return node;
		}

		classes.forEach(function(d) {
			find(d.name, d);
			});

		return map[""];
	}

	// Return a list of edges for the given array of nodes.
	$scope.packageEdges = function(nodes) {
		var map = {},
		    edges = [];

		// Compute a map from name to node.
		nodes.forEach(function(d) {
			map[d.name] = d;
		});

		// For each edge, construct a link from the source to target node.
		nodes.forEach(function(d) {
			if (d.edges) d.edges.forEach(function(i) {
				edges.push({source: map[d.name], target: map[i]});
			});
		});

		return edges;
	}

	$scope.mouseovered = function(d) {
		$scope.node_container
			.each(function(n) { n.target = n.source = false; });

		$scope.edge_container
			.classed("link--target", function(l) { if (l.target === d) return l.source.source = true; })
			.classed("link--source", function(l) { if (l.source === d) return l.target.target = true; })
			.filter(function(l) { return l.target === d || l.source === d; })
			.each(function() { this.parentNode.appendChild(this); });

		$scope.node_container
			.classed("node--target", function(n) { return n.target; })
			.classed("node--source", function(n) { return n.source; });
	}

	$scope.mouseouted = function(d) {
		$scope.edge_container
			.classed("link--target", false)
			.classed("link--source", false);

		$scope.node_container
			.classed("node--target", false)
			.classed("node--source", false);
	}

	/**Transform graph into bundle graph**/
	$scope.bundleGraphSelected = function(){
		$scope.resetButtons();

		//window.location.href = 'index_bundle.html';
		var diameter = w,
		        radius = diameter / 2,
			    //innerRadius = radius - 120;
			    innerRadius = radius - 180;

		var cluster = d3.layout.cluster()
			.size([360, innerRadius])
			.sort(null)
			.value(function(d) { return d.size; });

		var bundle = d3.layout.bundle();

		var line = d3.svg.line.radial()
			.interpolate("bundle")
			.tension(.85)
			.radius(function(d) { return d.y; })
			.angle(function(d) { return d.x / 180 * Math.PI; });

		// $scope.svg
		// 	.attr("width", diameter)
		// 	.attr("height", diameter)
		// 	.attr("transform", "translate(" + radius + "," + radius + ")");

		d3.select("svg")
		 	.attr("width", diameter)
		 	.attr("height", diameter);

		rect
			.attr("width", diameter)
			.attr("height", diameter);

		$scope.edge_container.selectAll("*").remove();
		$scope.node_container.selectAll("*").remove();

		d3.json("bundle_inputs/templated_"+$scope.archSelect+"_"+$scope.archstep+".json", function(error, classes) {
			if (error) throw error;

			$scope.dataset.nodes = cluster.nodes($scope.packageHierarchy(classes)),
			$scope.dataset.edges = $scope.packageEdges($scope.dataset.nodes);

			console.log($scope.dataset.nodes);
			console.log($scope.dataset.edges);

			$scope.edge_container = $scope.edge_container.selectAll(".link")
				.data(bundle($scope.dataset.edges))
				.enter().append("path")
				.each(function(d) { d.source = d[0], d.target = d[d.length - 1]; })
				.attr("class", "link")
				.attr("d", line)
				.attr("transform", "translate(" + radius + "," + radius + ")")
				.style("stroke", "steelblue")
				.style("stroke-opacity", ".4")
				.style("fill", "none")
				.style("pointer-events", "none");

			$scope.node_container = $scope.node_container.selectAll(".node")
				.data($scope.dataset.nodes.filter(function(n) { return !n.children; }))
				.enter().append("text")
				.attr("class", "node")
				.attr("dy", ".31em")
				.attr("transform", function(d) { return "translate(" + radius + "," + radius + ")rotate(" + (d.x - 90) + ")translate(" + (d.y + 8) + ",0)" + (d.x < 180 ? "" : "rotate(180)"); })
				.style("text-anchor", function(d) { return d.x < 180 ? "start" : "end"; })
				.text(function(d) { return d.key; })
				.on("mouseover", $scope.mouseovered)
				.on("mouseout", $scope.mouseouted);
		});
	}
	/**End bundle graph**/

	/**Transform graph into sankey graph**/
	$scope.sankeyGraphSelected = function(){
		$scope.resetButtons();
		window.location.href = 'index_sankey.html';
	}
	/**End sankey graph**/

	/**Transform graph into gantt graph**/
	$scope.ganttGraphSelected = function(){
		$scope.resetButtons();
		//window.location.href = 'index_gantt.html';
		$scope.displayGanttGraph();
	}

	$scope.displayGanttGraph = function(){
		// generate task list
		var tasks = [];
		var task_names = [];

		/*
		var start_time = Date.now();
		for(var i = 0; i < $scope.path.length; i++) {
			var node_delay = parseInt($scope.node_data[$scope.path[i][0]].delay, 10);
			var end_time = start_time + node_delay;
			tasks.push({
				"startDate": new Date(start_time),
				"endDate": new Date(end_time),
				"taskName" : $scope.path[i][0],
				"status" : $scope.node_data[$scope.path[i][0]].package
			});
			task_names.push($scope.path[i][0]);
			start_time = end_time;
		}	
		
		// plot gantt graph
		var gantt = d3.gantt().taskTypes(taskNames);
		gantt(tasks);
		*/


		var w = 800;
		var h = 400;


		 var svg = d3.selectAll("#chart")
		  //.selectAll("svg")
		  .append("svg")
		  .attr("width", w)
		  .attr("height", h)
		  .attr("class", "svg");

		  /*
		var start_time = Date.now();  
		for(var i = 0; i < $scope.path.length; i++) {
			var node_delay = parseInt($scope.node_data[$scope.path[i][0]].delay, 10);
			var end_time = start_time + node_delay;
			tasks.push({
				task : $scope.path[i][0],
				type : $scope.node_data[$scope.path[i][0]].package
				startTime: new Date(start_time),
				endTime: new Date(end_time),
			});
			task_names.push($scope.path[i][0]);
			start_time = end_time;
		}
		*/

		var taskArray = [
		{
		    task: "conceptualize",
		    type: "development",
		    startTime: "2013-1-28", //year/month/day
		    endTime: "2013-2-1",
		    details: "This actually didn't take any conceptualization"
		},

		{
		    task: "sketch",
		    type: "development",
		    startTime: "2013-2-1",
		    endTime: "2013-2-6",
		    details: "No sketching either, really"
		},

		{
		    task: "color profiles",
		    type: "development",
		    startTime: "2013-2-6",
		    endTime: "2013-2-9"
		},

		{
		    task: "HTML",
		    type: "coding",
		    startTime: "2013-2-2",
		    endTime: "2013-2-6",
		    details: "all three lines of it"
		},

		{
		    task: "write the JS",
		    type: "coding",
		    startTime: "2013-2-6",
		    endTime: "2013-2-9"
		},

		{
		    task: "advertise",
		    type: "promotion",
		    startTime: "2013-2-9",
		    endTime: "2013-2-12",
		    details: "This counts, right?"
		},

		{
		    task: "spam links",
		    type: "promotion",
		    startTime: "2013-2-12",
		    endTime: "2013-2-14"
		},
		{
		    task: "eat",
		    type: "celebration",
		    startTime: "2013-2-8",
		    endTime: "2013-2-13",
		    details: "All the things"
		},

		{
		    task: "crying",
		    type: "celebration",
		    startTime: "2013-2-13",
		    endTime: "2013-2-16"
		}];

		var dateFormat = d3.timeParse("%Y-%m-%d");

		var timeScale = d3.scaleTime()
		        .domain([d3.min(taskArray, function(d) {return dateFormat(d.startTime);}),
		                 d3.max(taskArray, function(d) {return dateFormat(d.endTime);})])
		        .range([0,w-150]);

		var categories = new Array();

		for (var i = 0; i < taskArray.length; i++){
		    categories.push(taskArray[i].type);
		}

		var catsUnfiltered = categories; //for vert labels

		categories = checkUnique(categories);

		makeGant(taskArray, w, h);

		var title = svg.append("text")
		              .text("Gantt Chart Process")
		              .attr("x", w/2)
		              .attr("y", 25)
		              .attr("text-anchor", "middle")
		              .attr("font-size", 18)
		              .attr("fill", "#009FFC");
	}

	/*
	//
	//  @author Dimitry Kudrayvtsev
	//  @version 2.0
	// 
	//  Ported to d3 v4 by Keyvan Fatehi on October 16th, 2016
	// 

	d3.gantt = function() {
	  var FIT_TIME_DOMAIN_MODE = "fit";
	  var FIXED_TIME_DOMAIN_MODE = "fixed";

	  var margin = {
	    top : 20,
	    right : 40,
	    bottom : 20,
	    left : 150
	  };
	  var timeDomainStart = d3.timeDay.offset(new Date(), -3);
	  var timeDomainEnd = d3.timeHour.offset(new Date(),+3);
	  var timeDomainMode = FIXED_TIME_DOMAIN_MODE;// fixed or fit
	  var taskTypes = [];
	  var taskStatus = [];
	  var height = document.body.clientHeight - margin.top - margin.bottom-5;
	  var width = document.body.clientWidth - margin.right - margin.left-5;

	  var tickFormat = "%H:%M";

	  var keyFunction = function(d) {
	    return d.startDate + d.taskName + d.endDate;
	  };

	  var rectTransform = function(d) {
	    return "translate(" + x(d.startDate) + "," + y(d.taskName) + ")";
	  };

	  var x,y,xAxis,yAxis;

	  initAxis();

	  var initTimeDomain = function() {
	    if (timeDomainMode === FIT_TIME_DOMAIN_MODE) {
	      if (tasks === undefined || tasks.length < 1) {
	        timeDomainStart = d3.timeDay.offset(new Date(), -3);
	        timeDomainEnd = d3.timeHour.offset(new Date(), +3);
	        return;
	      }
	      tasks.sort(function(a, b) {
	        return a.endDate - b.endDate;
	      });
	      timeDomainEnd = tasks[tasks.length - 1].endDate;
	      tasks.sort(function(a, b) {
	        return a.startDate - b.startDate;
	      });
	      timeDomainStart = tasks[0].startDate;
	    }
	  };

	 function initAxis() {
	    x = d3.scaleTime().domain([ timeDomainStart, timeDomainEnd ]).range([ 0, width ]).clamp(true);

	    y = d3.scaleBand().domain(taskTypes).range([ 0, height - margin.top - margin.bottom ]).padding(0.1);

	    xAxis = d3.axisBottom().scale(x).tickFormat(d3.timeFormat(tickFormat))
	      .tickSize(8).tickPadding(8);

	    yAxis = d3.axisLeft().scale(y).tickSize(0);
	  };

	  function gantt(tasks) {

	    initTimeDomain();
	    initAxis();

	    var svg = d3.select("body")
	      .append("svg")
	      .attr("class", "chart")
	      .attr("width", width + margin.left + margin.right)
	      .attr("height", height + margin.top + margin.bottom)
	      .append("g")
	      .attr("class", "gantt-chart")
	      .attr("width", width + margin.left + margin.right)
	      .attr("height", height + margin.top + margin.bottom)
	      .attr("transform", "translate(" + margin.left + ", " + margin.top + ")");
	    
	    svg.selectAll(".chart")
	      .data(tasks, keyFunction).enter()
	      .append("rect")
	      .attr("rx", 5)
	      .attr("ry", 5)
	      .attr("class", function(d){ 
	        if(taskStatus[d.status] == null){ return "bar";}
	        return taskStatus[d.status];
	      }) 
	      .attr("y", 0)
	      .attr("transform", rectTransform)
	      .attr("height", function(d) { return 70; })
	      .attr("width", function(d) { 
	        return (x(d.endDate) - x(d.startDate)); 
	      });

	      svg.append("g")
	        .attr("class", "x axis")
	        .attr("transform", "translate(0, " + (height - margin.top - margin.bottom) + ")")
	        .transition()
	        .call(xAxis);

	      svg.append("g").attr("class", "y axis").transition().call(yAxis);

	      return gantt;

	  };

	  gantt.redraw = function(tasks) {

	    initTimeDomain();
	    initAxis();

	    var svg = d3.select("svg");
	    var ganttChartGroup = svg.select(".gantt-chart");
	    var rect = ganttChartGroup.selectAll("rect").data(tasks, keyFunction);

	    rect.enter()
	      .insert("rect",":first-child")
	      .attr("rx", 5)
	      .attr("ry", 5)
	      .attr("class", function(d){ 
	        if(taskStatus[d.status] == null){ return "bar";}
	        return taskStatus[d.status];
	      }) 
	      .transition()
	      .attr("y", 0)
	      .attr("transform", rectTransform)
	      .attr("height", function(d) { return y.bandwidth(); })
	      .attr("width", function(d) { 
	         return (x(d.endDate) - x(d.startDate)); 
	      });

	      rect.merge(rect).transition()
	      	.attr("transform", rectTransform)
		      .attr("height", function(d) { return y.bandwidth(); })
	        .attr("width", function(d) { 
	           return (x(d.endDate) - x(d.startDate));
	        });

	        rect.exit().remove();

	        svg.select(".x").transition().call(xAxis);
	        svg.select(".y").transition().call(yAxis);

	        return gantt;
	  };

	  gantt.margin = function(value) {
	    if (!arguments.length)
	      return margin;
	    margin = value;
	    return gantt;
	  };

	  gantt.timeDomain = function(value) {
	    if (!arguments.length)
	      return [ timeDomainStart, timeDomainEnd ];
	    timeDomainStart = +value[0], timeDomainEnd = +value[1];
	    return gantt;
	  };

	  //
	  // @param {string}
	  //                vale The value can be "fit" - the domain fits the data or
	  //                "fixed" - fixed domain.
	  //
	  gantt.timeDomainMode = function(value) {
	    if (!arguments.length)
	      return timeDomainMode;
	    timeDomainMode = value;
	    return gantt;

	  };

	  gantt.taskTypes = function(value) {
	    if (!arguments.length)
	      return taskTypes;
	    taskTypes = value;
	    return gantt;
	  };

	  gantt.taskStatus = function(value) {
	    if (!arguments.length)
	      return taskStatus;
	    taskStatus = value;
	    return gantt;
	  };

	  gantt.width = function(value) {
	    if (!arguments.length)
	      return width;
	    width = +value;
	    return gantt;
	  };

	  gantt.height = function(value) {
	    if (!arguments.length)
	      return height;
	    height = +value;
	    return gantt;
	  };

	  gantt.tickFormat = function(value) {
	    if (!arguments.length)
	      return tickFormat;
	    tickFormat = value;
	    return gantt;
	  };

	  return gantt;
	};
	*/

	/**End gantt graph**/

	/**Dataset Manipulation**/
	$scope.addnode = function(name) {
		$scope.dataset.nodes.push([name]);
		$scope.dataset.nodes[$scope.nodecount].size = 1;
		$scope.nodecount++;
		return $scope.nodecount-1;
	};

	$scope.findnode = function(nodename) {
		for (var n=0; n < $scope.dataset.nodes.length; n++)
		if ($scope.dataset.nodes[n][0] == nodename) {
			return [$scope.dataset.nodes[n], n];
		}
		console.log("can't find " + nodename)
		return ["", -1];
	};

	$scope.removeFromDataset = function(links, nodes){
		links.forEach(function(l){
			var found = -1;
			for (var i = 0; i < $scope.dataset.edges.length; i++) {
				var cur = $scope.dataset.edges[i];
				if (cur.source[0] == l.__data__.source[0] && cur.target[0] == l.__data__.target[0]) {
					found = i;
				}
			}

			if (found > -1) {
				var r = $scope.dataset.edges.splice(found, 1);
			}
		});

		nodes.forEach(function(n){
			var sn = $scope.findnode(n.__data__[0]);
			if(sn[1] > -1){
				$scope.dataset.nodes.splice(sn[1], 1);
			}
		});
	}
	/**End dataset manipulation**/

	/**Enabling and disabling zoom**/

	//zoom object for adjusting the button text according to zoom state
	$scope.zoomObj = {};
	$scope.zoomObj["state"] = 0;
	$scope.zoomObj["buttonText"] = "Disable Zoom";

	$scope.removeZoom = function(){
		if($scope.zoomObj["state"] == 0){
			zoom.on("zoom", null);
			$scope.zoomObj["buttonText"] = "Enable Zoom";
			$scope.zoomObj["state"] = 1;
		} else{
			zoom.on("zoom", zoomed);
			$scope.zoomObj["buttonText"] = "Disable Zoom";
			$scope.zoomObj["state"] = 0
		}
	}
	/**End zoom disable and enable**/

	/**Help button**/
	$scope.help = function(){
		var data = "<b>Create Edge</b> <br> Click on a start node and then click on an end node. "+
			" The arrow that appears is to show where the edge will point. <br> <br>" +
			" <b>Create Node</b> <br> Fill in all parts of the form, then submit it. Click on the page where you want the new node to be. <br><br>" +
			" <b>Delete Edge</b> <br> Click on delete edge and then click on edge you want to delete. <br><br>" +
			" <b>Delete Node</b> <br> Click on delete node and then click on node you want to delete. <br><br>" +
			" <b>Undo Last Delete</b> <br> Undo the last delete edge or delete node. If no deletes occurred, nothing will happen. <br><br>" +
			" <b>Cancel Action</b> <br> If any action button (i.e. create node, delete edge) is selected, remove state of being in this action mode <br><br>"+
			" <b>Bundle Graph</b> <br> Creates a graph showing the connections between each node on current graph. <br><br>" +
			" <b>Network Graph</b> <br> Creates a graph showing the connectivity/network flow in the dependency graph. <br><br>" +
			" <b>Gantt Graph</b> <br> Creates a Gantt graph showing the latency breakdown in the dependency graph. <br><br>" +
			" <b>Show Export</b> <br> Shows the entire graph as a JSON, including any additions or deletions made. <br><br>" +
			" <b>Disable Zoom</b> <br> Temporarily stops zooming function. Can be re-enabled.";
		$scope.myWindow = window.open("data:text/html," + encodeURIComponent(data), "_blank", "width=500, height=200");
		$scope.myWindow.focus();
	}
	/**End help button**/

	/**Util methods**/
	$scope.getDependencies = function(nodeName){
		return $scope.links[0].filter(function(l){
			return l.__data__.source[0] === nodeName;
		});
	}

	function neighboring(a, b) {
		return $scope.linked[a.index + ',' + b.index];
	}

	$scope.removeNum = function(s) {
		return s.replace(/[0-9]/g, '');
	}

	//resets buttons such that none are pressed and empties the new node form. zoom is renabled
	$scope.resetButtons = function(){
		for (var i = 1; i < 5; i++){
			document.getElementById('button'+i).style.borderStyle = "";
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
				zone: undefined
			}

		$scope.node = angular.copy(defaultForm);

		document.getElementById('nodeFormContainer').style.display = "none";

		if($scope.zoomObj["state"] != 0){
			zoom.on("zoom", zoomed);
			$scope.zoomObj["buttonText"] = "Disable Zoom";
			$scope.zoomObj["state"] = 0
		}
	}

	/**Utility methods for adjusting node size according to edges**/
	$scope.subtractSize = function(edges) {
		edges.forEach(function(l){
			var s = l.__data__.source[0];
			var t = l.__data__.target[0];

			var sn = $scope.findnode(s);
			var tn = $scope.findnode(t);
			if(sn[0] != ""){
				if ($scope.dataset.nodes[sn[1]].size > 0) {
					$scope.dataset.nodes[sn[1]].size--;
				}
			}
			if(tn[0] != ""){
				if ($scope.dataset.nodes[tn[1]].size > 0) {
					$scope.dataset.nodes[tn[1]].size--;
				}
			}
		})
	}

	$scope.addSize = function(edges) {
		edges.forEach(function(l){
			var s = l.__data__.source[0];
			var t = l.__data__.target[0];

			var sn = $scope.findnode(s);
			var tn = $scope.findnode(t);
			if(sn[0] != ""){
				$scope.dataset.nodes[sn[1]].size++;
			}
			if(tn[0] != ""){
				$scope.dataset.nodes[tn[1]].size++;
			}
		})
	}
	/**End node size utility methods**/


	// gantt chart method
	function makeGant(tasks, pageWidth, pageHeight){

	    var barHeight = 20;
	    var gap = barHeight + 4;
	    var topPadding = 75;
	    var sidePadding = 75;

	    var colorScale = d3.scaleLinear()
	            .domain([0, categories.length])
	            .range(["#00B9FA", "#F95002"])
	            .interpolate(d3.interpolateHcl);

	    makeGrid(sidePadding, topPadding, pageWidth, pageHeight);
	    drawRects(tasks, gap, topPadding, sidePadding, barHeight, colorScale, pageWidth, pageHeight);
	    vertLabels(gap, topPadding, sidePadding, barHeight, colorScale);
	}


	function drawRects(theArray, theGap, theTopPad, theSidePad, theBarHeight, theColorScale, w, h){

	    var bigRects = svg.append("g")
	            .selectAll("rect")
	         .data(theArray)
	         .enter()
	         .append("rect")
	         .attr("x", 0)
	         .attr("y", function(d, i){
	                return i*theGap + theTopPad - 2;
	        })
	         .attr("width", function(d){
	                return w-theSidePad/2;
	         })
	         .attr("height", theGap)
	         .attr("stroke", "none")
	         .attr("fill", function(d){
	            for (var i = 0; i < categories.length; i++){
	                    if (d.type == categories[i]){
	                        return d3.rgb(theColorScale(i));
	                    }
	            }
	         })
	         .attr("opacity", 0.2);


	             var rectangles = svg.append('g')
	             .selectAll("rect")
	             .data(theArray)
	             .enter();


	         var innerRects = rectangles.append("rect")
	                             .attr("rx", 3)
	                             .attr("ry", 3)
	                             .attr("x", function(d){
	                                return timeScale(dateFormat(d.startTime)) + theSidePad;
	                                })
	                             .attr("y", function(d, i){
	                                    return i*theGap + theTopPad;
	                            })
	                             .attr("width", function(d){
	                                    return (timeScale(dateFormat(d.endTime))-timeScale(dateFormat(d.startTime)));
	                             })
	                             .attr("height", theBarHeight)
	                             .attr("stroke", "none")
	                             .attr("fill", function(d){
	                                for (var i = 0; i < categories.length; i++){
	                                        if (d.type == categories[i]){
	                                            return d3.rgb(theColorScale(i));
	                                        }
	                                }
	                             })


	                     var rectText = rectangles.append("text")
	                                 .text(function(d){
	                                    return d.task;
	                                 })
	                                 .attr("x", function(d){
	                                    return (timeScale(dateFormat(d.endTime))-timeScale(dateFormat(d.startTime)))/2 + timeScale(dateFormat(d.startTime)) + theSidePad;
	                                    })
	                                 .attr("y", function(d, i){
	                                        return i*theGap + 14+ theTopPad;
	                                })
	                                 .attr("font-size", 11)
	                                 .attr("text-anchor", "middle")
	                                 .attr("text-height", theBarHeight)
	                                 .attr("fill", "#fff");


	    rectText.on('mouseover', function(e) {
	     // console.log(this.x.animVal.getItem(this));
	                                 var tag = "";

	                     if (d3.select(this).data()[0].details != undefined){
	                        tag = "Task: " + d3.select(this).data()[0].task + "<br/>" + 
	                                    "Type: " + d3.select(this).data()[0].type + "<br/>" + 
	                                    "Starts: " + d3.select(this).data()[0].startTime + "<br/>" + 
	                                    "Ends: " + d3.select(this).data()[0].endTime + "<br/>" + 
	                                    "Details: " + d3.select(this).data()[0].details;
	                     } else {
	                        tag = "Task: " + d3.select(this).data()[0].task + "<br/>" + 
	                                    "Type: " + d3.select(this).data()[0].type + "<br/>" + 
	                                    "Starts: " + d3.select(this).data()[0].startTime + "<br/>" + 
	                                    "Ends: " + d3.select(this).data()[0].endTime;
	                     }
	                     var output = document.getElementById("tag");

	                        var x = this.x.animVal.getItem(this) + "px";
	                        var y = this.y.animVal.getItem(this) + 25 + "px";

	                     output.innerHTML = tag;
	                     output.style.top = y;
	                     output.style.left = x;
	                     output.style.display = "block";
	                 }).on('mouseout', function() {
	                     var output = document.getElementById("tag");
	                     output.style.display = "none";
	                             });


	    innerRects.on('mouseover', function(e) {
	     //console.log(this);
	                     var tag = "";

	                     if (d3.select(this).data()[0].details != undefined){
	                        tag = "Task: " + d3.select(this).data()[0].task + "<br/>" + 
	                                    "Type: " + d3.select(this).data()[0].type + "<br/>" + 
	                                    "Starts: " + d3.select(this).data()[0].startTime + "<br/>" + 
	                                    "Ends: " + d3.select(this).data()[0].endTime + "<br/>" + 
	                                    "Details: " + d3.select(this).data()[0].details;
	                     } else {
	                        tag = "Task: " + d3.select(this).data()[0].task + "<br/>" + 
	                                    "Type: " + d3.select(this).data()[0].type + "<br/>" + 
	                                    "Starts: " + d3.select(this).data()[0].startTime + "<br/>" + 
	                                    "Ends: " + d3.select(this).data()[0].endTime;
	                     }
	                     var output = document.getElementById("tag");

	                     var x = (this.x.animVal.value + this.width.animVal.value/2) + "px";
	                     var y = this.y.animVal.value + 25 + "px";

	                     output.innerHTML = tag;
	                     output.style.top = y;
	                     output.style.left = x;
	                     output.style.display = "block";
	                 }).on('mouseout', function() {
	                     var output = document.getElementById("tag");
	                     output.style.display = "none";

	     });
	}


	function makeGrid(theSidePad, theTopPad, w, h){

	    var xAxis = d3.axisBottom(timeScale)
	            .ticks(d3.timeDay, 1)
	            .tickSize(-h+theTopPad+20, 0, 0)
	            .tickFormat(d3.timeFormat('%d %b'));

	    var grid = svg.append('g')
	            .attr('class', 'grid')
	            .attr('transform', 'translate(' +theSidePad + ', ' + (h - 50) + ')')
	            .call(xAxis)
	            .selectAll("text")  
	                            .style("text-anchor", "middle")
	                            .attr("fill", "#000")
	                            .attr("stroke", "none")
	                            .attr("font-size", 10)
	                            .attr("dy", "1em");
	}

	function vertLabels(theGap, theTopPad, theSidePad, theBarHeight, theColorScale){
	  var numOccurances = new Array();
	  var prevGap = 0;

	  for (var i = 0; i < categories.length; i++){
	    numOccurances[i] = [categories[i], getCount(categories[i], catsUnfiltered)];
	  }

	  var axisText = svg.append("g") //without doing this, impossible to put grid lines behind text
	   .selectAll("text")
	   .data(numOccurances)
	   .enter()
	   .append("text")
	   .text(function(d){
	    return d[0];
	   })
	   .attr("x", 10)
	   .attr("y", function(d, i){
	    if (i > 0){
	        for (var j = 0; j < i; j++){
	          prevGap += numOccurances[i-1][1];
	         // console.log(prevGap);
	          return d[1]*theGap/2 + prevGap*theGap + theTopPad;
	        }
	    } else{
	    return d[1]*theGap/2 + theTopPad;
	    }
	   })
	   .attr("font-size", 11)
	   .attr("text-anchor", "start")
	   .attr("text-height", 14)
	   .attr("fill", function(d){
	    for (var i = 0; i < categories.length; i++){
	        if (d[0] == categories[i]){
	        //  console.log("true!");
	          return d3.rgb(theColorScale(i)).darker();
	        }
	    }
	   });

	}

	//from this stackexchange question: http://stackoverflow.com/questions/1890203/unique-for-arrays-in-javascript
	function checkUnique(arr) {
	    var hash = {}, result = [];
	    for ( var i = 0, l = arr.length; i < l; ++i ) {
	        if ( !hash.hasOwnProperty(arr[i]) ) { //it works with objects! in FF, at least
	            hash[ arr[i] ] = true;
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


	/**End Utility Methods**/
});
