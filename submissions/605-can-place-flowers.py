class Solution:
    def canPlaceFlowers(self, flowerbed: List[int], n: int) -> bool:
        avail = 0
        if n == 0:
            return True
        if len(flowerbed) == 1:
            return flowerbed[0] == 0 and n == 1
        if flowerbed[0] == 0 and flowerbed[1] == 0:
            flowerbed[0] = 1
            avail += 1
        if flowerbed[len(flowerbed) - 1] == 0 and flowerbed[len(flowerbed) - 2] == 0:
            flowerbed[len(flowerbed) - 1] = 1
            avail += 1
        for i in range(1, len(flowerbed)):
            if flowerbed[i] == 0 and flowerbed[i-1] == 0 and flowerbed[i+1] == 0:
                flowerbed[i] = 1
                avail += 1
        if(avail >= n):
            return True
        else:
            return False
                 
            
