
class TestOperateurs

    public TestOperateurs() {}
    
    
    private integer eval(integer v, string n)
    {
        prints(n);
        prints(" a ete evalue\n");
        return(v);
    }
    
    private string boolToString(integer v)
    {
        if v then
            return("vrai");
        else
            return("faux");
        endif
    }


    public void testEgalite()
    {
        VAR integer t;
        VAR string s;
        prints("Evaluation egalite:\n");
        
        prints("Entrez une chaine : \n"); s := getchar();
        prints("Entrez un entier : \n"); t := getint();
        
        prints("56 = ");
        printi(t);
        prints(" ? ");
        prints(boolToString(56 = t));
        prints("\n");
        
        prints("\"salut\" = \"");
        prints(s);
        prints("\" ? ");
        prints(boolToString("salut" = s));
        prints("\n");
        
        prints("1.50 = 1.50 ? ");
        prints(boolToString(1.50 = 1.50));
        prints("\n");
        
        
        prints("56 != ");
        printi(t);
        prints(" ? ");
        prints(boolToString(56 != t));
        prints("\n");
        
        prints("\"salut\" != \"");
        prints(s);
        prints("\" ? ");
        prints(boolToString("salut" != s));
        prints("\n");
        
        prints("1.50 != 1.50 ? ");
        prints(boolToString(1.50 != 1.50));
        prints("\n");
    }
    
    public void testEvalParesseuse()
    {
        VAR integer t;
        
        prints("Evaluation paresseuse:\n");
        
        prints("1 (a) ET 0 (b) :\n");
        t := eval(1, "a") & eval(0, "b");
        
        prints("0 (a) ET 1 (b) :\n");
        t := eval(0, "a") & eval(1, "b");
        
        prints("1 (a) OU 0 (b) :\n");
        t := eval(1, "a") || eval(0, "b");
        
        prints("0 (a) OU 1 (b) :\n");
        t := eval(0, "a") || eval(1, "b");

    }

    
    
    public void main()
    {
        testEvalParesseuse();
        testEgalite();
    }

end
