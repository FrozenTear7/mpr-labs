import time

from pyspark.sql import SparkSession
import os
import random


def inside(p):
    x, y = random.random(), random.random()
    return x*x + y*y < 1


if __name__ == '__main__':
    if os.name == 'nt':
        print('detected windows, setting HADOOP_HOME')
        os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'

    spark = SparkSession.builder \
        .master("local") \
        .appName("Estimate PI") \
        .getOrCreate()
    sc = spark.sparkContext
    NUM_SAMPLES = 100000

    count = sc.parallelize(range(0, NUM_SAMPLES)) \
        .filter(inside).count()

    print("Pi is roughly %f\n" % (4.0 * count / NUM_SAMPLES))

    time.sleep(300)

    spark.stop()
