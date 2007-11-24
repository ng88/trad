/*
    Fichier de test 1

*/

class MaClasse

    private integer a, b, c;
    private string d, e;
    public MaClasse test;

    private void main(string arg1, string arg2)
        return (1);
    
    public integer addition(integer a, integer b)
        return (a + b);    
        
    public string print()
    {
        
        VAR string ret;
        
        b := "salut \"vous\"\t!\t!\na la ligne\\ \\";
        
        ret := a + b + c + "ceci est une chaine";
        
        ret := ret + addition(a, 12);
        
        if 1+1 = 2 then
            a := 7;
        else
            a := b;
        endif
        
        while a do
            a := a - 1;
           
        while b do
        {
            b := b - 8;
            c := c - 1;
        }
        
        a.test(5).nimp(); 
        
        test.main("e", "f");
        
        return(ret);
    }


end
