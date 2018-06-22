package engine;
import java.time.LocalDate;
import java.lang.Integer;

//classe que identifica uma resposta
public class Answer{
    private Integer date; //data da resposta
    private Integer id_resp; //id da resposta
    private Integer id_question; //id da questão
    private Integer id_user; //id do user
    private int comentarios; //numero de comenantarios
    private int score; //score da resposta
    private int favoritos; //numero de favoritos da resposta

    //Construtor vazio de uma resposta
    public Answer(){    
        this.date = 0;
        this.id_resp = 0;
        this.id_question = 0;
        this.id_user = 0;
        this.comentarios = 0;
        this.score = 0;
        this.favoritos = 0;
    }

    //construtor por atribuicao de uma resposta
    public Answer(Integer date, Integer id_resp, Integer id_question, Integer id_user, int comentarios, int score, int favoritos){
        this.date = date;
        this.id_resp = id_resp;
        this.id_question = id_question;
        this.id_user = id_user;
        this.comentarios = comentarios;
        this.score = score;
        this.favoritos = favoritos;
    }

    //construtor copia de uma resposta
    public Answer(Answer a){
        this.date = a.getdate();
        this.id_resp = a.getid_resp();
        this.id_question = a.getid_question();
        this.id_user = a.getid_user();
        this.comentarios = a.getcomentarios();
        this.score = a.getscore();
        this.favoritos = a.getfavoritos();
    }

    //Get da data da resposta
    
    public Integer getdate(){
        return this.date;
    }

    //get do id da resposta
    public Integer getid_resp(){
        return this.id_resp;
    }

    //get do id da questao a que responde
    public Integer getid_question(){
        return this.id_question;
    }

    //get do id do user que fez resposta
    public Integer getid_user(){
        return this.id_user;
    }

    //get do numero de comentarios da resposta
    public int getcomentarios(){
        return this.comentarios;
    }

    //get do score da resposta
    public int getscore(){
        return this.score;
    }

    //get do numero de favoritos da resposta
    public int getfavoritos(){
        return this.favoritos;
    }

    //Set da data da resposta
    public void setDate(Integer date){
        this.date = date;
    }

    //set do id da resposta
    public void setId_resp(Integer id_resp){
        this.id_resp = id_resp;
    }

    //set do id da questao a que responde
    public void setId_question(Integer id_question){
        this.id_question = id_question;
    }

    //set do id do user da resposta
    public void setId_user(Integer id_user){
        this.id_user = id_user;
    }

    //set do numero de comentarios da resposta
    public void setComentarios(int comentarios){
        this.comentarios = comentarios;
    }

    //set do score da resposta
    public void setScore(int score){
        this.score = score;
    }

    //set do numero de favoritos da resposta
    public void setFavoritos(int favoritos){
        this.favoritos = favoritos;
    }

    //toString da resposta

     public String toString(){
        StringBuilder sb =new StringBuilder();

        sb.append("Date of publication(int format): ");
        sb.append(this.date + "\n");
        sb.append("Id of this answer: ");
        sb.append(this.id_resp + "\n");
        sb.append("Id of the respectively question: ");
        sb.append(this.id_question + "\n");
        sb.append("User's Id : ");
        sb.append(this.id_user + "\n");
        sb.append("Number of comments: ");
        sb.append(this.comentarios + "\n");
        sb.append("Score: ");
        sb.append(this.score + "\n");
        sb.append("Number of favourites: ");
        sb.append(this.favoritos + "\n");

        return sb.toString();
    }

    //Equals entre respostas
      public boolean equals(Object o){
        int i=0,valor = 1;

        if(o==this) return true;

        if((o==null) || (o.getClass() != this.getClass()))
            return false;
        
        Answer c = (Answer) o;

        return (super.equals(c)
                && this.getdate().equals(c.getdate())
                && this.getid_resp().equals(c.getid_resp())
                && this.getid_question().equals(c.getid_question())
                && this.getid_user().equals(c.getid_user())
                && this.getcomentarios() == c.getcomentarios()
                && this.getscore() == c.getscore() 
                && this.getfavoritos() == c.getfavoritos()); 
    }

    //Clone da resposta
    public Answer clone(){
        return new Answer(this);    
    }
}
