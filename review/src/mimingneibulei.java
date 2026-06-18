
    // 1. 定义一个抽象类
    abstract class Animal {
        abstract void makeSound();
    }

    public class mimingneibulei {
        public static void main(String[] args) {
            // 2. 匿名内部类：继承Animal抽象类，实现抽象方法
            Animal cat = new Animal() {
                @Override
                void makeSound() {
                    System.out.println("匿名内部类：yaleiyalei");
                }
            };
            // 3. 调用方法
            cat.makeSound();
        }
    }
//匿名内部类的意义在于即用即建

