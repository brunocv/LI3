package engine;

//classe que identifica uma tag
public class Tag{

    public Integer id; //id da tag
    public String tag; //nome da tag
    public int conta; //numero de vezes que aparece (query 11)

    //Construtores vazio das tags
    public Tag(){
    	this.id = 0;
    	this.tag = "";
        this.conta = 0;
    }

    //Construtor por atribuicao das tags
    public Tag(Integer id, String tag,int conta){
    	this.id = id;
    	this.tag = tag;
        this.conta=conta;
    }

    //construtor copia das tags
    public Tag(Tag t){
    	this.id = t.getid();
    	this.tag = t.gettag();
        this.conta= t.getConta();
    }

    //Getters

    //get id da tag
    public Integer getid(){
        return this.id;
    }

    //get nome da tag
    public String gettag(){
        return this.tag;
    }

    //get do contador da tag
    public int getConta(){
        return this.conta;
    }

    //Setters

    //set id da tag
    public void setId(Integer id) {
        this.id = id;
    }

    //set nome da tag
    public void setTag(String tag){
    	this.tag = tag;
    }

    //set do contador da tag
    public void setConta (int conta){
        this.conta = conta;
    }

    //toString da tag
     public String toString(){
        StringBuilder sb =new StringBuilder();

        sb.append("Id da Tag: ");
        sb.append(this.id + "\n");
        sb.append("Tag: ");
        sb.append(this.tag + "\n");
        sb.append("Contador: ");
        sb.append(this.conta + "\n");

        return sb.toString();
    }
    
    //Equals de tags
      public boolean equals(Object o){
        int i=0,valor = 1;

        if(o==this) return true;

        if((o==null) || (o.getClass() != this.getClass()))
            return false;
        
        Tag c = (Tag) o;

        return (super.equals(c) 
                && this.getid().equals(c.getid())  
                && this.gettag().equals(c.gettag())
                && this.getConta() == c.getConta());
    }

    //Clone de tag
    public Tag clone(){
        return new Tag(this);    
    }
}
