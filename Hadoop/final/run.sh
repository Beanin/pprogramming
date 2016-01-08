rm -rf res
mkdir res 
hadoop fs -rm -r tmp 
hadoop fs -rm -r outdata
hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
-input A \
-input B \
-output tmp \
-mapper mapA.py \
-reducer reduce.py \
-file mapA.py \
-file reduce.py \
-file /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar

hadoop jar /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar \
-input tmp \
-output outdata \
-mapper passive.py \
-reducer redfinal.py \
-file redfinal.py \
-file passive.py \
-file /opt/cloudera/parcels/CDH/lib/hadoop-mapreduce/hadoop-streaming.jar
hadoop fs -rm -r tmp
hadoop fs -get outdata/* res
