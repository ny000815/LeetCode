class Solution {
    public int countStudents(int[] students, int[] sandwiches) {
        Queue<Integer> peopleQueue = new LinkedList<>();
        Stack<Integer> sandwichStack = new Stack<>();
        for (int i = 0; i < students.length; i++) peopleQueue.offer(students[i]);
        for (int i = sandwiches.length-1; i >= 0; i--) sandwichStack.push(sandwiches[i]);

        int rotationCount = 0;
        while (peopleQueue.size() > 0 && rotationCount < peopleQueue.size()) {
            if (peopleQueue.peek() == sandwichStack.peek()) {
                peopleQueue.poll();
                sandwichStack.pop();
                rotationCount = 0;
            }
            else {
                peopleQueue.offer(peopleQueue.poll());
                rotationCount++;
            }
        }
        return peopleQueue.size();

    }
}
