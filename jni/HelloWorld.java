package test;

public class HelloWorld
{
    public static void main(String[] args)
    {
        System.loadLibrary("HelloWorld");
        printHello();
    }
    public static native final void printHello();
}

class HelloDDI
{
    public void javahello(){
        System.out.println("hello ddi!");
    }
}
