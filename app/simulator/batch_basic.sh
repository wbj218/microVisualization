app=$1
echo $app
time=$(date +'%Y_%m_%d_%H_%M')
echo $time

python simulator.py $app'_0.json' www 2 > '/filer-01/qsim/logs/'$time'_'$app'_2'.out
echo done
python simulator.py $app'_0.json' www 5 > '/filer-01/qsim/logs/'$time'_'$app'_5'.out
echo done
python simulator.py $app'_0.json' www 10 > '/filer-01/qsim/logs/'$time'_'$app'_10'.out
echo done
python simulator.py $app'_0.json' www 50 > '/filer-01/qsim/logs/'$time'_'$app'_50'.out
echo done
python simulator.py $app'_0.json' www 100 > '/filer-01/qsim/logs/'$time'_'$app'_100'.out
echo done
python simulator.py $app'_0.json' www 200 > '/filer-01/qsim/logs/'$time'_'$app'_200'.out
echo done
python simulator.py $app'_0.json' www 300 > '/filer-01/qsim/logs/'$time'_'$app'_300'.out
echo done
python simulator.py $app'_0.json' www 400 > '/filer-01/qsim/logs/'$time'_'$app'_400'.out
echo done
python simulator.py $app'_0.json' www 500 > '/filer-01/qsim/logs/'$time'_'$app'_500'.out
echo done
python simulator.py $app'_0.json' www 600 > '/filer-01/qsim/logs/'$time'_'$app'_600'.out
echo done
python simulator.py $app'_0.json' www 700 > '/filer-01/qsim/logs/'$time'_'$app'_700'.out
echo done
python simulator.py $app'_0.json' www 800 > '/filer-01/qsim/logs/'$time'_'$app'_800'.out
echo done
python simulator.py $app'_0.json' www 900 > '/filer-01/qsim/logs/'$time'_'$app'_900'.out
echo done
python simulator.py $app'_0.json' www 1000 > '/filer-01/qsim/logs/'$time'_'$app'_1000'.out
echo done
python simulator.py $app'_0.json' www 1500 > '/filer-01/qsim/logs/'$time'_'$app'_1500'.out
echo done
python simulator.py $app'_0.json' www 2000 > '/filer-01/qsim/logs/'$time'_'$app'_2000'.out
echo done
python simulator.py $app'_0.json' www 2500 > '/filer-01/qsim/logs/'$time'_'$app'_2500'.out
echo done
python simulator.py $app'_0.json' www 3000 > '/filer-01/qsim/logs/'$time'_'$app'_3000'.out
echo done
python simulator.py $app'_0.json' www 3500 > '/filer-01/qsim/logs/'$time'_'$app'_3500'.out
echo done
python simulator.py $app'_0.json' www 4000 > '/filer-01/qsim/logs/'$time'_'$app'_4000'.out
echo done
python simulator.py $app'_0.json' www 4500 > '/filer-01/qsim/logs/'$time'_'$app'_4500'.out
echo done
python simulator.py $app'_0.json' www 5000 > '/filer-01/qsim/logs/'$time'_'$app'_5000'.out
echo done
python simulator.py $app'_0.json' www 5500 > '/filer-01/qsim/logs/'$time'_'$app'_5500'.out
echo done
python simulator.py $app'_0.json' www 6000 > '/filer-01/qsim/logs/'$time'_'$app'_6000'.out
echo done
python simulator.py $app'_0.json' www 6500 > '/filer-01/qsim/logs/'$time'_'$app'_6500'.out
echo done
python simulator.py $app'_0.json' www 7000 > '/filer-01/qsim/logs/'$time'_'$app'_7000'.out
echo done
python simulator.py $app'_0.json' www 7500 > '/filer-01/qsim/logs/'$time'_'$app'_7500'.out
echo done
python simulator.py $app'_0.json' www 8000 > '/filer-01/qsim/logs/'$time'_'$app'_8000'.out
echo done
python simulator.py $app'_0.json' www 8500 > '/filer-01/qsim/logs/'$time'_'$app'_8500'.out
echo done
python simulator.py $app'_0.json' www 9000 > '/filer-01/qsim/logs/'$time'_'$app'_9000'.out
echo done
python simulator.py $app'_0.json' www 9500 > '/filer-01/qsim/logs/'$time'_'$app'_9500'.out
echo done
python simulator.py $app'_0.json' www 10000 > '/filer-01/qsim/logs/'$time'_'$app'_10000'.out
echo done all
