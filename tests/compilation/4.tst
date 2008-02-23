class A
public integer d;
public A(){}
public integer test1(real y) { return(y+9);}
end
class B
public A ma;
public B(){}
public A getA(integer a) { return(ma); }
end
class MaClasse

public integer c;

public MaClasse() {}
    public integer ajout(integer a, integer b)
    {
	VAR A mc;
	VAR B mc2;
	VAR integer d;
	c := 4;
	ajout(4, 3);
	mc.test1(0.01);
	/*d := mc2.getA(ajout(4, 3)).test1(0.9);*/
	d := mc2.getA(8).test1(0.9);
	return(a+b+c+mc.d+mc2.ma.d);

    }
    public void main()
    {

        VAR integer a, b, c, d;
	VAR MaClasse mc;

	if "sa\nlut" then
	a:=1;
	endif

        if 1 = 3-2 then
            a := 5;
        else
        {
            VAR string s1, s2;
            b := 6;
            c := 7;

            while 1 do
                a := 2 + 7 * 8;

        }
        endif

    }
end
