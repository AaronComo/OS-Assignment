import java.util.Arrays;
import java.util.Scanner;

public class Fibonacci extends Thread {
    private static int[] dp;

    Fibonacci(int len) {
        dp = new int[len];
    }

    @Override
    public void run() {
        super.run();
        dp[0] = 0;
        dp[1] = 1;
        for (int i = 2; i < dp.length; i++) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
    }

    public int[] returnData() {
        return dp;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int len = in.nextInt();
        Fibonacci f = new Fibonacci(len);
        f.run();
        System.out.println(Arrays.toString(f.returnData()));
        in.close();
    }
}
