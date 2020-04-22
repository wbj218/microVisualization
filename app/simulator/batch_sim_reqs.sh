app=$1
num=$2
echo $app
time=$(date +'%Y_%m_%d_%H_%M')
echo $time

tmp_dir='/filer-01/qsim/logs/'$time'_'$app''
mkdir $tmp_dir

filename=$tmp_dir'/'$time'_'$app'_2'.out
python simulator.py $app'_'$num'.json' 2 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_5'.out
python simulator.py $app'_'$num'.json' 5 > $filename
echo done $filename 
filename=$tmp_dir'/'$time'_'$app'_10'.out
python simulator.py $app'_'$num'.json' 10 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_50'.out
python simulator.py $app'_'$num'.json' 50 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_100'.out
python simulator.py $app'_'$num'.json' 100 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_200'.out
python simulator.py $app'_'$num'.json' 200 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_300'.out
python simulator.py $app'_'$num'.json' 300 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_400'.out
python simulator.py $app'_'$num'.json' 400 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_500'.out
python simulator.py $app'_'$num'.json' 500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_600'.out
python simulator.py $app'_'$num'.json' 600 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_700'.out
python simulator.py $app'_'$num'.json' 700 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_800'.out
python simulator.py $app'_'$num'.json' 800 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_900'.out
python simulator.py $app'_'$num'.json' 900 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_1000'.out
python simulator.py $app'_'$num'.json' 1000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_1500'.out
python simulator.py $app'_'$num'.json' 1500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_2000'.out
python simulator.py $app'_'$num'.json' 2000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_2500'.out
python simulator.py $app'_'$num'.json' 2500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_3000'.out
python simulator.py $app'_'$num'.json' 3000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_3500'.out
python simulator.py $app'_'$num'.json' 3500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_4000'.out
python simulator.py $app'_'$num'.json' 4000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_4500'.out
python simulator.py $app'_'$num'.json' 4500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_5000'.out
python simulator.py $app'_'$num'.json' 5000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_5500'.out
python simulator.py $app'_'$num'.json' 5500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_6000'.out
python simulator.py $app'_'$num'.json' 6000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_6500'.out
python simulator.py $app'_'$num'.json' 6500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_7000'.out
python simulator.py $app'_'$num'.json' 7000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_7500'.out
python simulator.py $app'_'$num'.json' 7500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_8000'.out
python simulator.py $app'_'$num'.json' 8000 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_8500'.out
python simulator.py $app'_'$num'.json' 8500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_9000'.out
python simulator.py $app'_'$num'.json' 9000 >$filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_9500'.out
python simulator.py $app'_'$num'.json' 9500 > $filename
echo done $filename
filename=$tmp_dir'/'$time'_'$app'_10000'.out
python simulator.py $app'_'$num'.json' 10000 > $filename
echo done $filename all
