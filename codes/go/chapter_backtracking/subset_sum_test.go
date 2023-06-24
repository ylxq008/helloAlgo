// File: subset_sum_test.go
// Created Time: 2023-06-24
// Author: Reanon (793584285@qq.com)

package chapter_backtracking

import (
	"fmt"
	"strconv"
	"testing"

	. "github.com/krahets/hello-algo/pkg"
)

func TestSubsetSumI(t *testing.T) {
	/* 初始化二叉树 */
	nums := []int{3, 4, 5}
	target := 9
	res := subsetSumI(nums, target)

	fmt.Printf("target = " + strconv.Itoa(target) + "输入数组 nums = ")
	PrintSlice(nums)

	fmt.Println("\n所有和等于 " + strconv.Itoa(target) + " 的子集 res = ")
	for i := range res {
		PrintSlice(res[i])
	}
}
