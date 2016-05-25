#! /bin/sh

# merge-sales.sh
#
# 结合与业务员数据

# 删除注释排序数据文件
sed '/^#/d' quotas | sort > quotas.sorted
sed '/^#/d' sales  | sort > sales.sorted

# 以第一个键值作结合，将输出结果产生至标准输出
#join quotas.sorted sales.sorted
join sales.sorted quotas.sorted


# 删除缓存文件
rm quotas.sorted sales.sorted
