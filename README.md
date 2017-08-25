

## Start NetflixMicroservices

### 1. Initialize docker containers
Initialize the container on ath1 so that you don't have to change the IP address and ports in the microservice source code. 
Open the NetflixMicroservices directory. 

Run
```
cd docker
vim setup_compose_review.py
```
In the last line

```
sp.call ("docker run --cpuset-cpus=19 -d -p 32800:80 --name nginx-php-fpm -v /home/yg397/Research/NetflixMicroservices/nginx-php/html:/var/www/html -v /home/yg397/Research/NetflixMicroservices/nginx-php/conf:/etc/nginx/sites-enabled richarvey/nginx-php-fpm:latest", shell = True)
```
Change `/home/yg397/Research/NetflixMicroservices/nginx-php/html` and `/home/yg397/Research/NetflixMicroservices/nginx-php/conf` to your own absolute path of `nginx-php/html` and `nginx-php/conf`, save and close the script.

Run
```
python setup_compose_review.py
```
It should initialize nginx-php-fpm, memcached and mongoDB instances in docker. 

### 2. Build executables
Build executables on ath2 so that you don't have install all dependencies yourself. 

Open the NetflixMicroservices directory. 

Run
```
cmake ./
make -j8
```
Then, all executables should be in `./bin` directory. 

### 3. Run executables
Run the executables on ath2 so that you don't have to change the IP address and ports in the microservice source code. 

Open the NetflixMicroservices directory. 

Run
```
cd scripts
./run_compose_review.sh
```
The script runs all the microservices needed for composing reviews as will as the workload generator server. 

### 4. Run the workload driver
Open the NetflixMicroservices directory. 

Run
```
cd generator
python3 Loader.py num_of_generators(4 as default) num_of_total_reqs qps
```
Indicate num_of_generators, num_of_total_reqs and qps yourself.
