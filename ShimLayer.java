public class ShimLayer {

    static{
        System.loadLibrary("ShimLayer");
    }

    public static native void run();

    public static void main(String[]args) {
        new ShimLayer().run();
    }

}