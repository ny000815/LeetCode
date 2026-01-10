class Solution:
    def reverseVowels(self, s: str) -> str:
        v = []
        vowels = set('aAiIuUeEoO')
        count = 0
        for l in s:
            if (l in vowels):
                v.append(l)
                count += 1

        res = []

        for l in s:
            if (l in vowels):
                res.append(v.pop())
            else:
                res.append(l)

        return("".join(res))
