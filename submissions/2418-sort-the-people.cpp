class Solution {
public:
    vector<string> sortPeople(vector<string>& names, vector<int>& heights) {
        quickSort(names, heights, 0, heights.size() - 1);
        return names;
    }

    void quickSort(vector<string>& names, vector<int>& heights, int low, int high) {
        if (low < high) {
            int pivotIndex = partition(names, heights, low, high);
            quickSort(names, heights, low, pivotIndex - 1);
            quickSort(names, heights, pivotIndex + 1, high);
        }
    }

    int partition(vector<string>& names, vector<int>& heights, int low, int high) {
        int pivot = heights[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (heights[j] > pivot) {
                i++;
                swap(heights[i], heights[j]);
                swap(names[i], names[j]);
            }
        }
        swap(heights[i + 1], heights[high]);
        swap(names[i + 1], names[high]);
        return i + 1;
    }
};
