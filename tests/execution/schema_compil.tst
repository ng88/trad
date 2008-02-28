
class A

    private integer a, b;

    public A()
        prints("construit un A...\n");

    public string nom()
        return("A");

    public void printNom()
    {
	prints("nom de la classe de l'instance=");
	prints(nom());
	prints("\n");
    }
    
    public integer ajouter(integer x, integer y)
            return (x + y + a + b);

end

class B
	inherit A
        
        private string a, b;

        public B()
            prints("construit un B...\n");
    
        public string nom()
            return("B");
end



class Test

    public Test() {}

    public void main()
    {
        VAR A a1, a2;
        
	a1 := new B();
	a2 := new A();

        a1.printNom();
	a2.printNom();

    }

end


