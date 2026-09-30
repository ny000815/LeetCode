class Solution {
    public int[] twoSum(int[] nums, int target) {
        HashMap<Integer, Integer> seen = new HashMap<>();
        for(int i = 0; i < nums.length; ++i) {
            int num = nums[i];
            if (seen.containsKey(target - num)) return new int[]{i, seen.get(target - num)};
            seen.put(num, i);
        }
        return new int[]{};
    }
}
