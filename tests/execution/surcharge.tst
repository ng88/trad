
class A

    public A() {}
    
    public void test()
    {
        prints("je suis test sans parametre\n");
    }
    
    public void test(integer a)
    {
        prints("je suis test avec 1 param entier = ");
        printi(a);
        prints("\n");
    }
    
    public void test(string a)
    {
        prints("je suis test avec 1 param chaine = ");
        prints(a);
        prints("\n");
    }
    
    public void test(integer a, integer b)
    {
        prints("je suis test avec 2 params entiers = ");
        printi(a);
        prints(", ");
        printi(b);
        prints("\n");
    }
    
    public void test(integer a, string b)
    {
        prints("je suis test avec 2 params entier, chaine = ");
        printi(a);
        prints(", ");
        prints(b);
        prints("\n");
    }

end


class TestSurcharge

    public TestSurcharge() {}
    
    
    public void main()
    {
        VAR A a;
        a := new A();
        
        a.test();
        a.test(5);
        a.test("salut");
        a.test(5, "salut");
        a.test(5, 6);
        
    }

end
