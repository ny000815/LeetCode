class Solution {
    public List<String> letterCombinations(String digits) {
        ArrayList<String> ans = new ArrayList<>();

        HashMap<Character, String> digitToLetters = new HashMap<>();
        digitToLetters.put('2', "abc");
        digitToLetters.put('3', "def");
        digitToLetters.put('4', "ghi");
        digitToLetters.put('5', "jkl");
        digitToLetters.put('6', "mno");
        digitToLetters.put('7', "pqrs");
        digitToLetters.put('8', "tuv");
        digitToLetters.put('9', "wxyz");

        backtrack(digits, 0, "", ans, digitToLetters);
        return ans;
    }
    private void backtrack(String digits, int index, String combo, List<String> res, Map<Character, String> digitToLetters) {
        if (index == digits.length()) {
            res.add(combo);
            return;
        }
        String letters = digitToLetters.get(digits.charAt(index));
        for (int i = 0; i < letters.length(); i++) {
            backtrack(digits, index+1, combo + letters.charAt(i), res, digitToLetters);
        }
    }
}
