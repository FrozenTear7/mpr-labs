import os
import time
import re
from pyspark.sql import SparkSession


def get_experiment_id(line):
    experiment_id = re.search(r'/experiments/(\w|\d)+/', line).group(0)[13:-1]
    return experiment_id


if __name__ == '__main__':
    if os.name == 'nt':
        print('detected windows, setting HADOOP_HOME')
        os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'

    spark = SparkSession.builder \
        .master("local") \
        .appName("Word Count") \
        .getOrCreate()
    sc = spark.sparkContext

    text_file = sc.textFile("text")
    valid_line_regex = re.compile(r'.*/experiments/(\w|\d)+/experiment_stats.*')

    top10Experiments = text_file.flatMap(lambda line: line.split('\n')) \
        .filter(lambda x: valid_line_regex.match(x)) \
        .map(lambda x: get_experiment_id(x)) \
        .map(lambda x: (x, 1)) \
        .reduceByKey(lambda a, b: a + b) \
        .sortBy(lambda x: x[1], False) \
        .map(lambda x: x[0]) \
        .take(10)

    print(top10Experiments)

    time.sleep(300)

    spark.stop()
