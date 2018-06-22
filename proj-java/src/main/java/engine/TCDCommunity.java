package engine;

import common.Pair;
import li3.TADCommunity;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.time.LocalDate;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Collectors;
import java.util.ArrayList;
import java.util.Comparator;
import java.lang.Integer;
import java.lang.Long;
import java.lang.String;
import java.lang.StringBuilder;

//classe em que sao implementadas as queries
public class TCDCommunity implements TADCommunity {

    private Map <Integer,Answer> answers; //guarda as respostas (id da resposta, resposta)
    private Map <Integer,Question> questions; //guarda as questoes (id da questao, questao)
    private Map <Integer,User> users; //guarda os users (id do user,user)
    private Map <Integer,Tag> tags; //guarda as tags (id da tag, tag)
    
    //construtor vazio de TCDExample
    public TCDCommunity(){
    	this.answers = new HashMap<>();
    	this.questions = new HashMap<>(); 
    	this.users = new HashMap<>(); 
    	this.tags = new HashMap<>(); 
    
    }
    
    //load das informacoes para as estruturas
    public void load(String dumpPath) {
        
        String path0 = dumpPath.concat("Posts.xml");
        String path1 = dumpPath.concat("Users.xml");
        String path2 = dumpPath.concat("Tags.xml");

        File fXmlFile0 = new File(path0);
        File fXmlFile1 = new File(path1);
        File fXmlFile2 = new File(path2);

        new ParseLoad().ParseUsers(fXmlFile1, this.users);
        new ParseLoad().ParsePosts(fXmlFile0,this.answers,this.questions,this.users);
        new ParseLoad().ParseTags(fXmlFile2, this.tags);
        
    }
    
    //Get das respostas
    public Map<Integer,Answer> getAnswers() {
        return this.answers.values().stream().collect(Collectors.toMap((e) -> e.getid_resp(),(e) -> e.clone())); 
    }

    //get das questoes
    public Map<Integer,Question> getQuestions() {
        return this.questions.values().stream().collect(Collectors.toMap((e) -> e.getid(),(e) -> e.clone())); 
    }

    //get dos users
    public Map<Integer,User> getUsers() {
        return this.users.values().stream().collect(Collectors.toMap((e) -> e.getid(),(e) -> e.clone())); 
    }

    //get das tags
    public Map<Integer,Tag> getTags() {
        return this.tags.values().stream().collect(Collectors.toMap((e) -> e.getid(),(e) -> e.clone())); 
    }
    
    // Query 1
    public Pair<String,String> infoFromPost(long id) {
        String title="";
        String name="";

        if (getQuestions().containsKey((int)id)){
            
           Question q = getQuestions().get((int)id);
           title = q.gettitulo();
           int id1 = q.getid_user();

           if (getUsers().containsKey(id1)){
                User u = getUsers().get(id1);
                name = u.getusername();
            }
        }

        else if (getAnswers().containsKey((int)id)){
            Answer a = getAnswers().get((int)id);

            Question q = getQuestions().get (a.getid_question());
            
            title = q.gettitulo();
            int id1 = q.getid_user();

            User u = getUsers().get(id1);
            name = u.getusername();
        }
 
        return new Pair<>(title, name);
    }

    // Query 2
    public List<Long> topMostActive(int N) {
        List<Long> array = new ArrayList<>();
 
        List<User> l = new ArrayList<>(getUsers().values());
        
        l.sort(new Comparator<User>() {
            public int compare(User u1, User u2) {
                if(u1.getpost_count() == u2.getpost_count()){
                    return 0;
                }
                else if (u1.getpost_count() > u2.getpost_count())
                    return -1;
                else return 1;
            }
        });
        
        l.subList(0,N);

        int i=0;
        for (User u : l) {
            if (i<N){
                Long z = new Long(u.getid().longValue());
                array.add( z );
            }
            else break;
            i++;
        }
        
        return array;
    }

    // Query 3
    public Pair<Long,Long> totalPosts(LocalDate begin, LocalDate end) {
        long perguntas=0;
        long respostas=0;
        long comeca= begin.getYear() * 10000 + begin.getMonthValue() * 100 + begin.getDayOfMonth();
        long acaba = end.getYear() * 10000 + end.getMonthValue() * 100 + end.getDayOfMonth();
        List<Question> l1 = new ArrayList<>(getQuestions().values());
        List<Answer> l2 = new ArrayList<>(getAnswers().values());

        for( Question q :l1){
            if (q.getdate().longValue() >= comeca && q.getdate().longValue() <= acaba) perguntas++;
        }

        for(Answer a :l2){
            if (a.getdate().longValue() >= comeca && a.getdate().longValue() <= acaba) respostas++;
        }
        Long z = new Long(perguntas );
        Long x = new Long(respostas );

        return new Pair<>(z,x);
    }

    // Query 4
    public List<Long> questionsWithTag(String tag, LocalDate begin, LocalDate end) {

        long comeca= begin.getYear() * 10000 + begin.getMonthValue() * 100 + begin.getDayOfMonth();
        long acaba = end.getYear() * 10000 + end.getMonthValue() * 100 + end.getDayOfMonth();
        
        List<Question> l = new ArrayList<>();
        List<Question> l2 = new ArrayList<>(getQuestions().values());
        for(Question q :l2){
            if (q.getdate().longValue() >= comeca && q.getdate().longValue() <= acaba && q.gettags().contains("<"+tag+">")) l.add(q);
            
        }

        l.sort(new Comparator<Question>() {
            public int compare(Question q1, Question q2) {
                if(q1.getdate().equals(q2.getdate())){
                    return 0;
                }
                else if (q1.getdate().longValue() > q2.getdate().longValue())
                    return -1;
                else return 1;
            }
        });
 
        List<Long> array = new ArrayList<>();
        
        for (Question q : l) {
            Long z = new Long(q.getid().longValue());
            array.add( z );
        }

        return array;
    }

    // Query 5
    public Pair<String, List<Long>> getUserInfo(long id) {
        String bio="";

        int idx= (int)id;
        if (getUsers().containsKey(idx)){
            User u = getUsers().get(idx);
            bio = u.getshort_bio();
        }
        
        //ficar com as questions dele e ordenar
        List<Question> l2 = new ArrayList<>(getQuestions().values());
        List<Question> l = new ArrayList<>();
        for(Question q : l2){
            if (q.getid_user().equals(idx)) l.add(q);
        }

        l.sort(new Comparator<Question>() {
            public int compare(Question q1, Question q2) {
                if(q1.getdate().equals(q2.getdate())){
                    return 0;
                }
                else if (q1.getdate().longValue() > q2.getdate().longValue())
                    return -1;
                else return 1;
            }
        });
         
        //ficar com as answers dele e ordenar
        List<Answer> d = new ArrayList<>();
        List<Answer> d2 = new ArrayList<>(getAnswers().values());
        for(Answer a : d2){
            if (a.getid_user().equals(idx)) d.add(a);
        }

        d.sort(new Comparator<Answer>() {
            public int compare(Answer a1, Answer a2) {
                if(a1.getdate().equals(a2.getdate())){
                    return 0;
                }
                else if (a1.getdate().longValue() > a2.getdate().longValue())
                    return -1;
                else return 1;
            }
        });
       
	int i=0;
        int ques=0;
        int resp=0;
        int dim1=l.size();
        int dim2=d.size();
        List<Long> array = new ArrayList<>();

        for ( ;i<10; i++){

            if (dim1==0 && dim2==0) continue;
            
            if (dim1==0 && dim2>0){
                Answer a =d.get(resp);
                Long z = new Long(a.getid_resp().longValue());
                array.add( z );
                resp++;
                dim2--;
                continue;
            }
            if (dim2==0 && dim1>0){
                Question q =l.get(ques);
                Long z = new Long(q.getid().longValue());
                array.add( z );
                ques++;
                dim1--;
                continue;
            }
            
            Question q =l.get(ques);
            Answer a =d.get(resp);

            if (q.getdate().longValue() >= a.getdate().longValue()) {
                Long z = new Long(q.getid().longValue());
                array.add( z );
                ques++;
                dim1--;
                continue;
            }
            if (q.getdate() < a.getdate()){
                Long z = new Long(a.getid_resp().longValue());
                array.add( z );
                resp++;
                dim2--;
                continue;
            }
        }

        return new Pair<>(bio,array);
    }

    // Query 6
    public List<Long> mostVotedAnswers(int N, LocalDate begin, LocalDate end) {
        long comeca= begin.getYear() * 10000 + begin.getMonthValue() * 100 + begin.getDayOfMonth();
        long acaba = end.getYear() * 10000 + end.getMonthValue() * 100 + end.getDayOfMonth();

        List<Answer> d2 = new ArrayList<>(getAnswers().values());
        List<Answer> d = new ArrayList<>();
        for(Answer a : d2){
            if (a.getdate().longValue() >= comeca && a.getdate().longValue() <= acaba) d.add(a);
        }
        
        d.sort(new Comparator<Answer>() {
            public int compare(Answer a1, Answer a2) {
                if( a1.getscore() == a2.getscore()){
                    return 0;
                }
                else if ( a1.getscore() > a2.getscore())
                    return -1;
                else return 1;
            }
        });

        d.subList(0,N);
        
        List<Long> array = new ArrayList<>();
        int i=0;
        for (Answer a : d) {
            if (i<N){
                Long z = new Long(a.getid_resp().longValue());
                array.add( z );
            }
            else break;
            i++;
        }
        

        return array;

    }

    // Query 7
    public List<Long> mostAnsweredQuestions(int N, LocalDate begin, LocalDate end) {
        long comeca= begin.getYear() * 10000 + begin.getMonthValue() * 100 + begin.getDayOfMonth();
        long acaba = end.getYear() * 10000 + end.getMonthValue() * 100 + end.getDayOfMonth();

        // Respostas no intervalo
        List<Answer> resps = getAnswers().values().stream()
			.filter(e -> e.getdate().longValue() >= comeca && e.getdate().longValue() <= acaba)
			.map(Answer::clone)
			.collect(Collectors.toList());
	
        // Questoes no intervalo com contador a zero (é clonado já)
	Map <Integer,Question> quests = getQuestions().values().stream()
			.filter(e -> e.getdate().longValue() >= comeca && e.getdate().longValue() <= acaba)
			.map(Question::clone)
			.collect(Collectors.toMap((e) -> e.getid(),(e) -> e.clone()));

	for (Map.Entry<Integer, Question> entrada : quests.entrySet()) {
            entrada.getValue().setRespostas_count(0);
        }
		
	List<Question> lq = getQuestions().values().stream()
			.filter(e -> e.getdate().longValue() >= comeca && e.getdate().longValue() <= acaba)
			.map(Question::clone)
			.collect(Collectors.toList());

	// Aumenta ao contador de respostas
        for (Answer a : resps) {
            if (quests.containsKey(a.getid_question().intValue()) == true) {
        	Question q = quests.get(a.getid_question().intValue());
        	q.setRespostas_count(q.getrespostas_count() + 1);
        	quests.put(q.getid(),q.clone());
        	}
        }

        List<Question> l = new ArrayList<>();
        
        for (Map.Entry<Integer, Question> entrada : quests.entrySet()) {
            Question e = entrada.getValue(); 
            l.add(e);
            l.sort(new Comparator<Question>() {
                public int compare(Question q1, Question q2) {
                    if(q1.getrespostas_count() == q2.getrespostas_count()){
                    	return 0;
                    }
                    else if (q1.getrespostas_count() > q2.getrespostas_count())
                    	    return -1;
                    else return 1;
            	    }
                });
            }

        List<Long> array = new ArrayList<>();
        int i=0;
        for (Question q : l) {
            if (i<N){
                Long z = new Long(q.getid().longValue());
                array.add( z );
            }
            else break;
            i++;
        }

        return array;
    }

        

    // Query 8
    public List<Long> containsWord(int N, String word) {
 
        List<Question> l1 = new ArrayList<>(getQuestions().values());
        List<Question> l = new ArrayList<>();
        for(Question q : l1){
		StringBuilder s2 =  new StringBuilder(" " + q.gettitulo() + " ");
		for (int i = 0; i < s2.length(); i++) {
			if ((s2.charAt(i) >= 'a' && s2.charAt(i) <= 'z') || (s2.charAt(i) >= 'A' && s2.charAt(i) <= 'Z')) s2.setCharAt(i, s2.charAt(i));			
			else s2.setCharAt(i, ' ');
		}
            String[] result = s2.toString().split("\\s");
            for (int x=0; x<result.length; x++){
             	if (word.equals(result[x])) {
			l.add(q);
			break;
		    }
            }
        }

        l.sort(new Comparator<Question>() {
            public int compare(Question q1, Question q2) {
                if(q1.getdate().equals(q2.getdate())){
                    return 0;
                }
                else if (q1.getdate().longValue() > q2.getdate().longValue())
                    return -1;
                else return 1;
            }
        });
   
        l.subList(0,N);

        List<Long> array = new ArrayList<>();
        int i=0;
        for (Question q : l) {
            if (i<N){
                Long z = new Long(q.getid().longValue());
                array.add( z );
            }
            else break;
            i++;
        }
     
        return array;
    }

    // Query 9
    public List<Long> bothParticipated(int N, long id1, long id2) {
        
         Map <Integer,Answer> getans = new HashMap<>(getAnswers());
         Map <Integer,Question> getquest = new HashMap<>(getQuestions());
        
        List<Question> l = new ArrayList<>();
        for(Map.Entry<Integer, Question> e : getquest.entrySet()){
            Question q = e.getValue();
            if (q.getid_user().equals((int)id1)) l.add(q);
        }

        for(Map.Entry<Integer, Answer> e : getans.entrySet()){
            Answer a = e.getValue();
            if (a.getid_user().equals((int)id1)){ 
                 if (getquest.containsKey( a.getid_question())){
                    Question q = getquest.get( a.getid_question());
                    l.add(q);
                 }
            }
        }


        List<Question> l2 = new ArrayList<>();
        for(Map.Entry<Integer, Question> e : getquest.entrySet()){
            Question q = e.getValue();
            if (q.getid_user().equals((int)id2)) l2.add(q);
        }

        for(Map.Entry<Integer, Answer> e : getans.entrySet()){
            Answer a = e.getValue();
            if (a.getid_user().equals((int)id2)){ 
                 if (getquest.containsKey( a.getid_question())){
                    Question q = getquest.get( a.getid_question());
                    l2.add(q);
                 }
            }
        }

        List<Question> l3 = new ArrayList<>();
        for( Question q : l){
            if (l2.contains(q)) l3.add(q);
        }

        l3.sort(new Comparator<Question>() {
            public int compare(Question q1, Question q2) {
                if(q1.getdate().equals(q2.getdate())){
                    return 0;
                }
                else if (q1.getdate().longValue() > q2.getdate().longValue())
                    return -1;
                else return 1;
            }
        });
   
        
        
        
        List<Long> array = new ArrayList<>();
        int i=0;
        for (Question q : l3) {
            if (i<N ){
                Long z = new Long(q.getid().longValue());
                if (array.contains(z)==false) array.add( z );
            }
            else break;
            i++;
        }

        return array;
        
    }

    // Query 10

    public double cotacao_resp(int id,Map<Integer, Answer> l2,Map<Integer,User> l3) {
        Answer a = l2.get(id).clone();
        User u = l3.get(a.getid_user()).clone();
        
        return a.getscore() * 0.65 + u.getreputation() * 0.25 + a.getcomentarios() * 0.1;
    }
    
    public long betterAnswer(long id) {
        Map<Integer,Answer> l2 = new HashMap<>(getAnswers());
        Map<Integer,User> l3 = new HashMap<>(getUsers());
        
        List<Answer> l = getAnswers().values().stream()
				.filter(e -> e.getid_question().equals((int)id))
				.map(Answer::clone)
				.collect(Collectors.toList());
		
	l.sort(new Comparator<Answer>() {
                public int compare(Answer m1, Answer m2) {
                    if(cotacao_resp(m1.getid_resp().intValue(),l2,l3) == cotacao_resp(m2.getid_resp().intValue(),l2,l3)){
                        return 0;
                    }
                    else if (cotacao_resp(m1.getid_resp().intValue(),l2,l3) > cotacao_resp(m2.getid_resp().intValue(),l2,l3))
                        return -1;
                    else return 1;
                }
            });
	    
        if (l.size() == 0) return -1;
		
        else return l.get(0).getid_resp().longValue();
    
    }
   
    // Query 11
    public List<Long> mostUsedBestRep(int N, LocalDate begin, LocalDate end) {

        int comeca= begin.getYear() * 10000 + begin.getMonthValue() * 100 + begin.getDayOfMonth();
        int acaba = end.getYear() * 10000 + end.getMonthValue() * 100 + end.getDayOfMonth();

        List <User> l = new ArrayList<>(getUsers().values());
        List<Question> quest = new ArrayList<>(getQuestions().values());

        l.sort(new Comparator<User>() {
            public int compare(User u1, User u2) {
                if(u1.getreputation() == u2.getreputation()){
                    return 0;
                }
                else if (u1.getreputation() > u2.getreputation())
                    return -1;
                else return 1;
            }
        });
        
        List <User>  x = new ArrayList<>(l.subList(0,N));
        
        Map<Integer, User> m= new HashMap<>();
        for(User u : x) 
            m.put(u.getid(), u.clone());

        List<Question> d = new ArrayList<>();
        for(Question q : quest){
            if (q.getdate() >= comeca && q.getdate() <= acaba && m.containsKey(q.getid_user())){ 
                d.add (q);
                }
        }
        
        List<Tag> t = new ArrayList<>(getTags().values());
        for ( Tag ta : t){
            for (Question q : d){
                if (q.gettags().contains("<"+ta.gettag()+">")) ta.setConta (ta.getConta() +1);
            }
        }

        t.sort(new Comparator<Tag>() {
            public int compare(Tag t1, Tag t2) {
                if(t1.getConta() == t2.getConta()){
                    return 0;
                }
                else if (t1.getConta() > t2.getConta())
                    return -1;
                else return 1;
            }
        });
        
        List<Long> array = new ArrayList<>();
        int i=0;
        for (Tag tag: t) {
            if (i<N){
                Long z = new Long(tag.getid().longValue());
                array.add( z );
            }
            else break;
            i++;
        }


        return array;

    }

    //funcao que limpa as estruturas (free mem)
    public void clear(){

        this.users.clear();
        this.answers.clear();
        this.questions.clear();
        this.tags.clear();

    }
}
