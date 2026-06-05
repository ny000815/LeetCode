class Solution {
    public String decodeString(String s) {
        Stack<Character> stack = new Stack<>();
        for (int i = 0; i < s.length(); i++) {
            if (s.charAt(i) == (']')) {
                List<Character> decodeString = new ArrayList<>();
                while (stack.peek() != '[') decodeString.add(stack.pop());
                stack.pop();
                int base = 1, num = 0;
                while (!stack.isEmpty() && Character.isDigit(stack.peek())) {
                    num = num + (stack.pop() - '0') * base;
                    base *= 10;
                }
                while(num>0){
                    for (int j = decodeString.size() - 1; j >= 0; j--) stack.push(decodeString.get(j));
                    num--;
                }
            } 
            else stack.push(s.charAt(i));
        } 
        char[] result = new char[stack.size()];
        for (int i = stack.size()-1; i >= 0; i--) result[i] = stack.pop();
        return new String(result);

    }
}
