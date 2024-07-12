class Solution {
public:
int maximumGain(string s, int x, int y) {
	int score = 0;
	string first, second;
	char a, b;

	if (x > y){
		first = "ab";
		second = "ba";
		a = 'a';
		b = 'b';
	}else{
		first = "ba";
		second = "ab";
		a = 'b';
		b = 'a';
		int temp = x;
		x = y;
		y = temp;
	}

	string stack;
	for (int i = 0; i < s.length(); i++){
		char c = s[i];
		if (stack.length() > 0 && stack[stack.length() - 1] == a && c == b) {
				stack.pop_back();
				score += x;
			} else{
				stack.push_back(c);
			}
	}

	s = stack;
	stack.clear();

	for (int i = 0; i < s.length(); i++){
		char c = s[i];
		if (stack.length() > 0 && stack[stack.length() - 1] == b && c == a) {
                stack.pop_back();
				score += y;
			} else{
				stack.push_back(c);
	        }
    }  
        return score;
    }
};
