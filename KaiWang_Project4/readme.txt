@Kai Wang
I write two test programs for different way of distribution, and I suppose the main idea of this project is to learn data sharing between processes. So I simplified the problem: all the test data have the same number of digits; and use insertion sort within each bucket, instead of using recursive bucket sort.

However, For larger amounts of data, I compared the difference in speedup by modifying the distribution. 

For files with filenames ending in x:
The test program distribute number by their first ONE digit,
for example, 1999 and 1111 are in the same bucket since the first
ONE digits of both are 1. As the result, there are 9 buckets for
numbers from 1000 to 9999

For files with filenames ending in xx:
The test program distribute number by their first TWO digits,
for example, 1999 and 1111 are NOT in the same bucket since the first
TWO digits of both are different (19 & 11). As the result, there are 90 buckets for
numbers from 1000 to 9999