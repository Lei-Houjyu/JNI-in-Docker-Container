public class ShimLayer {

    static{
        System.loadLibrary("ShimLayer");
    }

    public static native void run();

    public static native void invoke();

    public static void main(String[]args) {
        new ShimLayer().run();
    }

}