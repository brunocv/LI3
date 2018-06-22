package engine;

//classe que identifica um user
public class User{

    private Integer id;//id do user
    private String username; //nome do user
    private int post_count; //numero de posts do user
    private String short_bio; //biografia do user
    private int reputation; //reputacao do user

    //Construtor vazio de um user
    public User(){
    	this.id = 0;
    	this.username = "";
    	this.post_count = 0;
    	this.short_bio = "";
    	this.reputation = 0;
    }

    //construtor por atribuicao de um user
    public User(Integer id, String username, int post_count, String short_bio, int reputation){
    	this.id = id;
    	this.username = username;
    	this.post_count = post_count;
    	this.short_bio = short_bio;
    	this.reputation = reputation;
    }

    //construtor copia de um user
    public User(User u){
    	this.id = u.getid();
    	this.username = u.getusername();
    	this.post_count = u.getpost_count();
    	this.short_bio = u.getshort_bio();
    	this.reputation = u.getreputation();
    }

    //Getters

    //get do id do user
    public Integer getid(){
        return this.id;
    }

    //get do username do user
    public String getusername(){
        return this.username;
    }

    //get do numero de posts do user
    public int getpost_count(){
        return this.post_count;
    }

    //get da biografia do user
    public String getshort_bio(){
        return this.short_bio;
    }

    //get da reputacao do user
    public int getreputation(){
        return this.reputation;
    }

    //Set do id do user
    public void setId(Integer id){
        this.id = id;
    }

    //set do username do user
    public void setUsername(String username){
        this.username = username;
    }

    //set do numero de posts do user
    public void setPost_count(int post_count){
        this.post_count = post_count;
    }

    //set da biografia do user
    public void setShort_bio(String short_bio){
        this.short_bio = short_bio;
    }

    //set da reputacao do user
    public void setReputation(int reputation){
        this.reputation = reputation;
    }

    //toString de um user

     public String toString(){
        StringBuilder sb =new StringBuilder();

        sb.append("Id do user: ");
        sb.append(this.id + "\n");
        sb.append("Username: ");
        sb.append(this.username + "\n");
        sb.append("Number of posts : ");
        sb.append(this.post_count + "\n");
        sb.append("User's bio: ");
        sb.append(this.short_bio + "\n");
        sb.append("User's reputation: ");
        sb.append(this.reputation + "\n");

        return sb.toString();
    }
    
    //Equal entre users

       public boolean equals(Object o){
        int i=0,valor = 1;

        if(o==this) return true;

        if((o==null) || (o.getClass() != this.getClass()))
            return false;
        
        User c = (User) o;

        return (super.equals(c) 
                && this.getid().equals(c.getid()) 
                && this.getusername().equals(c.getusername())
                && this.getpost_count() == c.getpost_count() 
                && this.getshort_bio().equals(c.getshort_bio())
        		&& this.getreputation() == c.getreputation());
    }

    //Clone de um user
    public User clone(){
        return new User(this);    
    }
}
