class Solution {
public:
    void reverseString(string& s, int start, int end) {
        while (start < end) {
            char temp = s[start];
            s[start] = s[end];
            s[end] = temp;
            start++;
            end--;
        }
    }
    string reverseWords(string s) {
        int n = s.length();
        reverseString(s, 0, n - 1);
        int writeindex = 0;
        bool inWord = false;
        
        // Step 2: Reverse each word and remove extra spaces
        for (int readIndex = 0; readIndex < n; readIndex++) {
            if (s[readIndex] != ' ') {
                if (inWord) {
                    s[writeindex++] = s[readIndex];
                } else {
                    if (writeindex > 0) {
                        s[writeindex++] = ' ';
                    }
                    int wordStart = writeindex;
                    while (readIndex < n && s[readIndex] != ' ') {
                        s[writeindex++] = s[readIndex++];
                    }
                    reverseString(s, wordStart, writeindex - 1);
                    readIndex--;  // Adjust for the extra increment in the while loop
                    inWord = true;
                }
            } else {
                inWord = false;
            }
        }
        if (writeindex > 0 && s[writeindex - 1] == ' ') {
            writeindex--; 
        }
        s.resize(writeindex);
        return s;
    }
};
