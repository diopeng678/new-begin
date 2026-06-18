interface Greeting {
    void sayHello();
}

public class lambda {
    public static void main(String[] args) {
        // Lambda 简化写法（替代匿名内部类）
        Greeting greeting = () -> System.out.println("Lambda：你好，Java！");
        // 调用方法
        greeting.sayHello();
    }
}
