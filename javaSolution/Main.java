public class Main {
    public static void main(String[] args) {
        SharedResource sharedResource = new SharedResource();

        // Create multiple threads and start them
        Thread thread1 = new Thread(new Gate("East", sharedResource));
        Thread thread2 = new Thread(new Gate("West", sharedResource));

        thread1.start();
        thread2.start();
    }
}
