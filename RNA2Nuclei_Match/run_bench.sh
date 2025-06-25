#!/bin/bash

# 用法: ./run_bench.sh brute_force,kdtree,lsh

METHODS=${1:-brute_force,kdtree,lsh}
DATA_DIR=data
RESULTS_DIR=results
LOG_DIR=$RESULTS_DIR/logs

mkdir -p $LOG_DIR

IFS=',' read -ra METHOD_LIST <<< "$METHODS"

NUCLEI_FILE="$DATA_DIR/nuclei.csv"
SPOTS_FILE="$DATA_DIR/spots.csv"
DATASET_NAME="default"

for METHOD in "${METHOD_LIST[@]}"; do
    echo "运行算法: $METHOD"

    EXEC_PATH="build/$METHOD"
    OUT_FILE="$LOG_DIR/${METHOD}_${DATASET_NAME}.csv"

    $EXEC_PATH --nuclei "$NUCLEI_FILE" --spots "$SPOTS_FILE" --out "$OUT_FILE"


    # 计算 recall
    RECALL=$(python3 -c "
import pandas as pd
true_df = pd.read_csv('$SPOTS_FILE')
pred_df = pd.read_csv('$OUT_FILE')
merged = pd.merge(true_df, pred_df, on='spot_id', suffixes=('_true', '_pred'))
correct = (merged['nucleus_id_true'] == merged['nucleus_id_pred']).sum()
recall = correct / len(true_df)
print(round(recall, 4))
    ")

    echo "$DATASET_NAME,$METHOD,$LATENCY,$RECALL" >> "$RESULTS_DIR/results.csv"
done

echo "运行完成。所有结果已记录到 $RESULTS_DIR/results.csv"

