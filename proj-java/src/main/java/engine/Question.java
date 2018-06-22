package engine;
import java.time.LocalDate;

//classe que identifica uma questao
public class Question{

    private Integer id_user;//id do user
    private Integer date; //data da questao
    private Integer id; //id da questao
    private String titulo; //titulo da questao
    private String tags; // tags da questao
    private int votos; // numero de votos da questao
    private int respostas_count; //numero de respostas

    //Construtor vazio de uma questao
    public Question(){
    	this.id_user = 0; 
        this.date = 0;
        this.id = 0;
        this.titulo = "";
      	this.tags = "";
      	this.votos = 0;
      	this.respostas_count = 0;
    }

    //construtor por atribuicao de uma questao
    public Question(Integer id_user, Integer date, Integer id, String titulo, String tags, int votos, int respostas_count){
        this.id_user = id_user; 
        this.date = date;
        this.id = id;
        this.titulo = titulo;
      	this.tags = tags;
      	this.votos = votos;
      	this.respostas_count = respostas_count;
    }

    //construtor copia de uma questao
    public Question(Question q){
    	this.id_user = q.getid_user(); 
        this.date = q.getdate();
        this.id = q.getid();
        this.titulo = q.gettitulo();
      	this.tags = q.gettags();
      	this.votos = q.getvotos();
      	this.respostas_count = q.getrespostas_count();
    }

    //Get do id do user da questao

    public Integer getid_user(){
        return this.id_user;
    }

    //get da data da questao
    public Integer getdate(){
        return this.date;
    }

    //get do id da questao
    public Integer getid(){
        return this.id;
    }

    //get do titulo da questao
    public String gettitulo(){
        return this.titulo;
    }

    //get das tags da questao
    public String gettags(){
        return this.tags;
    }

    //get dos votos da questao
    public int getvotos(){
        return this.votos;
    }

    //get do numero de respostas da questao
    public int getrespostas_count(){
        return this.respostas_count;
    }

    //Set do id do user da questao

     public void setId_user(Integer id_user){
        this.id_user = id_user;
    }

    //set da data da questao
    public void setDate(int date){
        this.date = date;
    }

    // VER O PK DE ISTO ESTAR AQUI
    public void setShort_bio(Integer id){
        this.id = id;
    }

    //set do titulo da questao
    public void setTitulo(String titulo){
        this.titulo = titulo;
    }

    //set das tags da questao
    public void setTags(String tags){
        this.tags = tags;
    }

    //set dos votos da questao
    public void setVotos(int votos){
        this.votos = votos;
    }

    //set do numero de respostas da questao
    public void setRespostas_count(int respostas_count){
        this.respostas_count = respostas_count;
    }

    //toString da questao

     public String toString(){
        StringBuilder sb =new StringBuilder();

        sb.append("This question belong to user whith this Id: ");
        sb.append(this.id_user + "\n");
        sb.append("Date of publication(int format): ");
        sb.append(this.date + "\n");
        sb.append("Question's Id: ");
        sb.append(this.id + "\n");
        sb.append("Title: ");
        sb.append(this.titulo + "\n");
        sb.append("Tags of this questions: ");
        sb.append(this.tags + "\n");
        sb.append("Number of votes: ");
        sb.append(this.votos + "\n");
        sb.append("Answer's count: ");
        sb.append(this.respostas_count + "\n");

        return sb.toString();
    }

    //Equals entre questoes
      public boolean equals(Object o){
        int i=0,valor = 1;

        if(o==this) return true;

        if((o==null) || (o.getClass() != this.getClass()))
            return false;
        
        Question c = (Question) o;

        return (super.equals(c) 
                && this.getid_user().equals(c.getid_user())
                && this.getdate().equals(c.getdate())  
                && this.getid().equals(c.getid())  
                && this.gettitulo().equals(c.gettitulo())
        		&& this.gettags().equals(c.gettags())
				&& this.getvotos() == c.getvotos()  
				&& this.getrespostas_count() == c.getrespostas_count());
    }

    //Clone de uma questao
    public Question clone(){
        return new Question(this);    
    }
}
