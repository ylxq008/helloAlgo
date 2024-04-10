=begin
File: merge_sort.rb
Created Time: 2024-04-10
Author: junminhong (junminhong1110@gmail.com)
=end

### 合并左子数组和右子数组 ###
def merge(nums, left, mid, right)
  # 左子数组区间为 [left, mid], 右子数组区间为 [mid+1, right]
  # 创建一个临时数组tmp，用于存放合并后的结果
  tmp = Array.new(right - left + 1)
  # 初始化左子数组和右子数组的起始索引
  i, j, k = left, mid + 1, 0
  # 当左右子数组都还有元素时，进行比较并将较小的元素复制到临时数组中
  while i <= mid and j <= right
    if nums[i] <= nums[j]
      tmp[k] = nums[i]
      i += 1
    else
      tmp[k] = nums[j]
      j += 1
    end
    k += 1
  end
  # 将左子数组和右子数组的剩余元素复制到临时数组中
  while i <= mid
    tmp[k] = nums[i]
    i += 1
    k += 1
  end
  while j <= right
    tmp[k] = nums[j]
    j += 1
    k += 1
  end
  # 将临时数组tmp中的元素复制回原数组nums的对应区间
  (0...tmp.length).each do |k|
    nums[left + k] = tmp[k]
  end
end

### 归并排序 ###
def merge_sort(nums, left, right)
  # 终止条件
  # 当子数组长度为1时终止递归
  return if left >= right
  # 划分阶段
  # 计算中点
  mid = (left + right) / 2
  # 递归左子数组
  merge_sort(nums, left, mid)
  # 递归右子数组
  merge_sort(nums, mid + 1, right)
  # 合并阶段
  merge(nums, left, mid, right)
end

### Driver Code ###
nums = [7, 3, 2, 6, 0, 1, 5, 4]
merge_sort(nums, 0, nums.length - 1)
puts "归并排序完成后 nums = #{nums.inspect}"
