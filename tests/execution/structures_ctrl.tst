
class Test

    public Test() {}
    

    public void main()
    {
        VAR integer a;
        prints("Entrez un nombre:\n");
        a := getint();
        
        if a >= 5 then
        {
            while a != 0 do
            {
                prints("hop !\n");
                a := a - 1;
            }
        }
        endif

    }

end


