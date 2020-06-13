import java.io.IOException;

public class Test {
    public UniqueId data;

    public Test() {
        data = new UniqueId("id.dat");
        try{
            data.init(10000);
        } catch (IOException e){
            e.printStackTrace();
        }
        // 5 Threads erzeugen und starten
        Thread[] threads = new Thread[5];
        for (int i = 0; i < 5; i++){
            threads[i] = new DataThread(data, "Thread"+i);
            threads[i].start();
        }
    }

    public static void main(String[] args) {
        Test test = new Test();
    }

}
