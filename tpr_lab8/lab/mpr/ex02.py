import os
import random

from pyspark.ml.linalg import Vectors
from pyspark.ml.regression import LinearRegression
from pyspark.sql import SparkSession

if __name__ == '__main__':
    if os.name == 'nt':
        print('detected windows, setting HADOOP_HOME')
        os.environ['HADOOP_HOME'] = 'C:/hadoop/hadoop-2.7.1'
    spark = SparkSession.builder \
        .master("local") \
        .appName("regression") \
        .getOrCreate()

    dataset = [(2 * i + 1, Vectors.dense([i])) for i in range(40)]

    data = spark.createDataFrame(dataset, ["label", "features"]).randomSplit([3.0, 1.0])  # wagi 3, 1

    train = data[0]

    # train = train.rdd.map(lambda x: (x[0] + (random.random() * 2 - 1), x[1])).toDF(["label", "features"])

    test = data[1]

    # train.show()
    lr = LinearRegression(maxIter=50, regParam=0.01, fitIntercept=True, tol=1e-5)
    model = lr.fit(train)
    # model.transform(test).show()

    print("Coefficients: %s" % str(model.coefficients))
    print("Intercept: %s" % str(model.intercept))

    trainingSummary = model.summary

    print("RMSE: %f" % trainingSummary.rootMeanSquaredError)

    spark.stop()
