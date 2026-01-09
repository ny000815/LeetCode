class Solution(object):
    def mergeAlternately(self, word1, word2):
        len1, len2 = len(word1), len(word2)
        i = 0
        res = []
        while (i < len1 or i < len2):
            if(i < len1):
                res.append(word1[i])
            if(i < len2):
                res.append(word2[i])
            i += 1
        
        return "".join(res)
