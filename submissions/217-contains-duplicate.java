class Solution {
    public boolean containsDuplicate(int[] nums) {
        HashSet<Integer> numbers = new HashSet<Integer>();
        Boolean isValid = false;
        for (int i = 0; i < nums.length; i++) {
            if (numbers.contains(nums[i])) {
                isValid = true;
            } else {
                numbers.add(nums[i]);
            }
        }
        return isValid;
    }
}
