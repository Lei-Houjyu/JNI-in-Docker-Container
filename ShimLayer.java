public class ShimLayer {

    static{
        System.loadLibrary("ShimLayer");
    }

    public static native void run(long id);

    public static void main(String[]args) {
    	try {
        	new ShimLayer().run(Long.parseLong(args[0]));
    	} catch (Exception e) {
    		e.printStackTrace();
    	}
    }

}