import java.util.Random;

public class Gate implements Runnable{
    private SharedResource sharedResource;
    String name;
    public  Gate(String name, SharedResource sharedResource){
        this.name = name;
        this.sharedResource = sharedResource ;
    }

    public void checkIn(){
        this.sharedResource.increment();
        System.out.println("Check in from " + this.name + ". Total number is: " + this.sharedResource.getCount());
    }

    public void checkOut(){
        this.sharedResource.decrement();
        System.out.println("Check out from " + this.name + ". Total number is: " + this.sharedResource.getCount());
    }


    @Override
    public void run() {
        while(true){
            Random random = new Random();
            int delayMilliseconds = random.nextInt(10) + 1000;
            try {
                Thread.sleep(delayMilliseconds);
                boolean outOperation = random.nextBoolean();
                if (outOperation && this.sharedResource.getCount() > 0 ){
                    this.checkOut();
                } else {
                    this.checkIn();

                }

            } catch (InterruptedException e) {
                System.out.println("Entered Exception");
                Thread.currentThread().interrupt();
            }

        }
    }

}
