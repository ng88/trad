
class Forme

    public Forme() {}


    public integer nombreCotes() { return(0); }
    public real perimetre() { return(0.0); }
    public string nom() { return("???"); }

    public void printNom()
    {
	prints("nom de la forme=");
	prints(nom());
	prints("\n");
    }

end

class Triangle
	inherit Forme

    private real ca, cb, cc;

    /* on suppose les infos bonnes ... c'est juste un exemple :)*/
    public Triangle(real a, real b, real c)
    {
       ca := a;
       cb := b;
       cc := c;
    }


    public integer nombreCotes() { return(3); }
    public real perimetre() { return(ca + cb + cc); }
    public string nom() { return("triangle"); }

end

class Rectangle
	inherit Forme

    private real ca, cb;

    public Rectangle(real a, real b)
    {
       ca := a;
       cb := b;
    }


    public integer nombreCotes() { return(4); }
    public real perimetre() { return( (ca + cb) * 2); }
    public string nom() { return("rectangle"); }

end

class Carre
	inherit Rectangle


    public Carre(real a)
    {
	super(a, a);
    }

    public real perimetre() { return(4 * ca); }
    public string nom() { return("carre"); }

end


class TestPloy

    public TestPloy() {}

    public void main()
    {
	VAR Forme f1, f2, f3;

	f1 := new Carre(4.7);
	f2 := new Triangle(1.0, 5.0, 2.0);
	f3 := new Rectangle(4.7, 6.0);

        f1.printNom();
	f2.printNom();
	f3.printNom();

	printi(f1.nombreCotes());
	prints("\n");


	freeobj(f1);
	freeobj(f2);
	freeobj(f3);


    }

end


