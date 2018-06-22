package engine;
import java.io.File;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import java.lang.Math;
import java.util.Map;
import java.util.HashMap;

public class ParseLoad{

    //Faz o parsing das tags
   public void ParseTags(File inputFile, Map <Integer,Tag> tags){

      try {
         String tag = "";
         int id;
         int contador=0;

         DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
         DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
         
         Document doc = dBuilder.parse(inputFile);
         doc.getDocumentElement().normalize();
         
         NodeList nList = doc.getElementsByTagName("row");

         for (int temp = 0; temp < nList.getLength(); temp++){
            Node nNode = nList.item(temp);
            
            if (nNode.getNodeType() == Node.ELEMENT_NODE){
               Element eElement = (Element) nNode;

               id = Integer.parseInt(eElement.getAttribute("Id"));

               tag = eElement.getAttribute("TagName"); 
            
               Tag t = new Tag(id,tag,contador);
               
               tags.put(t.getid(),t);
            }
         }
      } catch (Exception e){
         e.printStackTrace();
      }
   }

   // Faz o parsing dos users
   public void ParseUsers(File inputFile, Map<Integer,User> users){

      try {
         String dispName,bio;
         int id,reputacao;

         DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
         DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
         
         Document doc = dBuilder.parse(inputFile);
         doc.getDocumentElement().normalize();
         
         NodeList nList = doc.getElementsByTagName("row");

         for (int temp = 0; temp < nList.getLength(); temp++){
            Node nNode = nList.item(temp);
            
            if (nNode.getNodeType() == Node.ELEMENT_NODE){
               Element eElement = (Element) nNode;

               id = Integer.parseInt(eElement.getAttribute("Id"));

               dispName = eElement.getAttribute("DisplayName"); 

               bio = eElement.getAttribute("AboutMe"); 

               reputacao = Integer.parseInt(eElement.getAttribute("Reputation"));
               
               int post_count = 0;

               User u = new User(id, dispName, post_count, bio, reputacao);
               users.put(u.getid(),u);
            }
         }
      } catch (Exception e){
         e.printStackTrace();
      }
   }

   //Faz o parsing dos posts (perguntas e respostas)
   public void ParsePosts(File inputFile, Map <Integer,Answer> answers, Map <Integer,Question> questions, Map <Integer,User> users){

      try {
         int typeId,id,answerCount,scoreq,scorea,ownerUserId,parentId,comentarios;
         String title,tags,dateQ, dateA;

         DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
         DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
         
         Document doc = dBuilder.parse(inputFile);
         doc.getDocumentElement().normalize();
         
         NodeList nList = doc.getElementsByTagName("row");

         for (int temp = 0; temp < nList.getLength(); temp++){
            Node nNode = nList.item(temp);
            
            if (nNode.getNodeType() == Node.ELEMENT_NODE){
               Element eElement = (Element) nNode;

               typeId = Integer.parseInt(eElement.getAttribute("PostTypeId"));

               if(typeId == 1){
                  id = Integer.parseInt(eElement.getAttribute("Id"));

                  if(eElement.getAttribute("Title") == null) title = "";
                  else{
                     title = eElement.getAttribute("Title");
                  }

                  answerCount = Integer.parseInt(eElement.getAttribute("AnswerCount"));

                  scoreq = Integer.parseInt(eElement.getAttribute("Score"));

                  dateQ = eElement.getAttribute("CreationDate");
                  dateQ = dateQ.substring(0, Math.min(dateQ.length(), 10));
                  dateQ = dateQ.replaceAll("-","");

                  Integer dateq = Integer.valueOf(dateQ);

                  tags = eElement.getAttribute("Tags");

                  if((eElement.getAttribute("OwnerUserId") == null) || (eElement.getAttribute("OwnerUserId") == "")) ownerUserId = -2;
                  else{   
                     ownerUserId = Integer.parseInt(eElement.getAttribute("OwnerUserId"));
                  }

                  Question q = new Question(ownerUserId, dateq, id, title, tags, scoreq, answerCount);
                  questions.put(q.getid(),q);

                  if (users.containsKey(ownerUserId)){
                    User u = users.get(ownerUserId);
                    u.setPost_count ( u.getpost_count() +1);
                  }
               }

               else if(typeId == 2){
                  dateA = eElement.getAttribute("CreationDate");
                  dateA = dateA.substring(0, Math.min(dateA.length(), 10));
                  dateA = dateA.replaceAll("-","");
                  
                  Integer datea = Integer.valueOf(dateA);

                  id = Integer.parseInt(eElement.getAttribute("Id"));

                  if((eElement.getAttribute("OwnerUserId") == null) || (eElement.getAttribute("OwnerUserId") == "")) ownerUserId = -2;
                  else{
                     ownerUserId = Integer.parseInt(eElement.getAttribute("OwnerUserId"));
                  }

                  parentId = Integer.parseInt(eElement.getAttribute("ParentId"));

                  comentarios = Integer.parseInt(eElement.getAttribute("CommentCount"));

                  scorea = Integer.parseInt(eElement.getAttribute("Score"));

                  int favoritos = 0;

                  Answer a = new Answer(datea, id, parentId, ownerUserId, comentarios, scorea, favoritos);
                  answers.put(a.getid_resp(),a);
                  
                  if (users.containsKey(ownerUserId)){
                     User u = users.get(ownerUserId);
                     u.setPost_count ( u.getpost_count() +1);
                  }
               }
            }
         }
      } catch (Exception e){
         e.printStackTrace();
      }
   }
    

}
