#include <iostream>
#include <string>
#include<ctime>
#include <vector>
#include<fstream>
class Memento; 
class SnapShot{
    private:
    std::string fileName;
    std::vector<std::string> text;
    public:
    SnapShot(std::string fileName, std::vector<std::string> text):fileName(fileName),text(text){}
    std::vector<std::string> getText(){
        return text;
    }
    void setText(std::vector<std::string> text){
        this->text = text;
    }
    std::string getFileName(){
        return fileName;
    }
    void setFileName(std::string name){
        this->fileName = name;
    }
    void getInfo(){
        for (std::string s : text){
            std::cout<< s << std::endl;
        }
    }
};
class GitCommit{
    private:
    int commitId;
    std::string commitMessage;
    time_t now;
    std::vector<SnapShot> snapshot;
    public:
    GitCommit(int id, std::string m,std::vector<SnapShot> snapshot): commitId(id), commitMessage(m), snapshot(snapshot) {
        now = time(0);
    }
    void getInfo(){
        char* dt = ctime(&now);
        std::cout << commitId << " " << commitMessage << " " << dt << std::endl;
        for (SnapShot s: snapshot){
            s.getInfo();
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};
class GitRepository{
    private:
    std::string repositoryName;
    std::vector<GitCommit> commits;
    public:
    GitRepository(std::string repositoryName, std::vector<GitCommit> commits): repositoryName(repositoryName), commits(commits){}
    void gitCommit(GitCommit commit){
        commits.push_back(commit);
    }
    void getInfo(){
        std::cout<< repositoryName << "\n";
        for (GitCommit g : commits){
            g.getInfo();
        }
    }
    std::string getRepositoryName() const { 
        return repositoryName; 
    }
    std::vector<GitCommit> getCommits() const { 
        return commits;
    }
    void setRepositoryName(const std::string& repositoryName) { this->repositoryName = repositoryName; }
    void setCommits(const std::vector<GitCommit>& commits) { this->commits = commits; }
    Memento createMemento();
    void restoreMemento(const Memento& memento);
};
class Memento{
    private:
    GitRepository rep;
    public:
    Memento(const GitRepository& rep): rep(rep){}
    GitRepository getRepository() const {
        return rep;
    }
    void setRepository(GitRepository rep){
        this->rep = rep;
    }
};
Memento GitRepository::createMemento(){
    return Memento(*this);
}
void GitRepository::restoreMemento(const Memento& memento) {
    GitRepository memRep = memento.getRepository();
    setRepositoryName(memRep.getRepositoryName());
    setCommits(memRep.getCommits());
}



int main(){
    std::vector<GitCommit> commits;
    std::vector<SnapShot> snapshots;
    std::vector<Memento> mementos;
    std::string command;
    GitRepository repos("Repos", commits);
    std::string commands[2];
    while(true){
        std::cout << "Enter a command: ";
        std::getline(std::cin, command);
        commands[0] = command.substr(0, command.find(" "));
        commands[1] = command.substr(command.find(" ")+1, command.length()-1);
        
        if (commands[0] == "q"){
            break;
        } else if(commands[0] == "add") {
            std::ifstream in;
            std::string fileName ="" + commands[1];
            //std::cout << fileName << std::endl;
            std::vector<std::string> textLines;
            std::string t1;
            in.open(fileName);
            if (in.is_open()){
                while(std::getline(in, t1)){
                    textLines.push_back(t1);
                }
            }
            in.close();
            SnapShot s1(fileName, textLines);
            snapshots.push_back(s1);
        } else if (commands[0] == "commit"){
            std::string name = commands[1];
            GitCommit commit1(std::rand(), name, snapshots); 
            repos.gitCommit(commit1);
            snapshots.clear();
        } else if (commands[0] == "create"){
            Memento mem = repos.createMemento();
            mementos.push_back(mem);
            snapshots.clear();
            commits.clear();
        } else if(commands[0] == "restore"){
            unsigned int memNum = std::stoi(commands[1]);
            try{
                Memento mem = mementos.at(memNum);
                repos.restoreMemento(mem);
            }
            catch(...){
                std::cout << "Wrong index \n";
            }
        } else if(commands[0] == "info") { 
            repos.getInfo();
        } else {
            std::cout<< "Wrong command \n";
        }
    }
    //repos.getInfo();

}