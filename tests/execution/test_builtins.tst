
class MaClasse

public MaClasse() {}

    public void main()
    {
	VAR string s1, ret;
	VAR integer n, f;

	prints("Entrez une chaine :\n");
	s1 := getchar();
	
	prints("Entrez une position de depart :\n");
	f := getint();
	
	prints("Entrez une longueur :\n");
	n := getint();
	
	ret := substring(s1, f, n);
	
	prints("Entree :\n");
	prints(s1);
	prints("Depart=");
	printi(f);
	prints(", len=");
	printi(n);
	prints("\nResultat :\n");
	prints(ret);
	
	freestr(s1);
	freestr(ret);
    }
end
