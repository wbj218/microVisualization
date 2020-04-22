#names=(netflix_1 netflix_2 netflix_3 netflix_4 netflix_5 netflix_6 netflix_7 netflix_8 netflix_9 netflixoss_0 netflixoss_1 netflixoss_2 netflixoss_3 netflixoss_4 netflixoss_5 netflixoss_6 netflixoss_7 netflixoss_8 netflixoss_9  edge_0 lamp_0 lamp_1 lamp_2 lamp_3 lamp_4 lamp_5 lamp_6 lamp_7 lamp_8 lamp_9 fsm_0 fsm_1 fsm_2 fsm_3 fsm_4 fsm_5 cassandra_0 cassandra_1 cassandra_2 cassandra_3 cassandra_4 cassandra_5 cassandra_6 cassandra_7 cassandra_8 cassandra_9 container_0 container_1 container_2 container_3 basic_0 migration_0 migration_1 migration_2 migration_3 migration_4 migration_5 migration_6 migration_7 migration_8 migration_9 riak_0 riak_1 riak_2 storage_0 storage_1 yogi_0 yogi_1 yogi_2 yogi_3)

#filename='~/mvisualization/app/bundle_inputs/'$app'_'$num'.json'
#for i in {1..69}
for jsonfile in "../inputs"/*.json
do
	#python2.7 simulator.py ${names[$i]}'.json' 10 10
	#echo done ${names[$i]}
	name=$(basename $jsonfile .json)
	python2.7 simulator.py ${name}'.json' 10 10
	echo done ${name}
done
echo done all
