#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    totalNumAgents=50000;
    minNumAgents=1;
    renderdAgents=minNumAgents;
    targetAgentsNum=minNumAgents;

    
    colorangle=160;
    
    for (int i=0;i<totalNumAgents;i++){
        
        //    circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
        
        
        shared_ptr<Agent> a(new Agent);
        a->setup();
        agents.push_back(a);
        
        // we are passing the size in as a normal x position
        float size = ofRandom(5);
        sizes.push_back(ofVec3f(size));
        colors.push_back(ofRandom(1));
        
        
    }
    
    pointsize=5;
    targetPointSize=5;
    
    
    // fill in the colors and vertices
    vbo.setNormalData(&sizes[0], totalNumAgents, GL_STATIC_DRAW);
    
    ofBackgroundHex(0x000000);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    // load the texure
    ofDisableArbTex();
    ofLoadImage(texture, "dot.png");
    shader.load("shader/shader");
    
    // set the camera distance
    camDist  = 1305;
    camera.setDistance(camDist);
    
    
    // fbos
    
    
    
    fbo.allocate(ofGetWidth()*2, ofGetHeight()*2, GL_RGB);
    
    fbo.begin();
    ofClear(0,0,0,0);
    fbo.end();
    
    
    
    
    gui.setup(); // most of the time you don't need a name
    //gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
    // gui.add(homeforce.setup("homeforce", 1, 0, 5));
    gui.add(wanderforce.setup("wanderforce", 0.5, 0.0f, 1.5f));
    gui.add(maxvol.setup("maxvol", 0.05, 0.001f, 0.07f));
    gui.add(fadespeed.setup("fadespeed", 10, 0.0f, 30.0f));
    gui.add(rotationforce.setup("rotation", 0.f, -.05f, 0.05f));
    gui.add(seekforce.setup("seekforce", 0.5, 0.0f, 1.5f));
    
    gui.add(wanderD.setup("wanderD", 500, 100.f, 2000.f));
    gui.add(wanderR.setup("wanderR", 50, 0.0f, 500.f));
    gui.add(change.setup("change", 0.1, 0.0f, 5.f));
    gui.add(maxspeed.setup("maxspeed", 4, 0, 20));
    
    
    gui.add(targetPointSize.setup("Point Size", 5, 1.0f, 200));

    

    
    
    gui.loadFromFile("settings.xml");
    
    bHide=true;
    
    gutter.set(100,100);
    
    
    int Xammount=ofGetWidth()*2/gutter.x;
    int Yammount=ofGetHeight()*2/gutter.y;
    
    int xPos=0;
    int yPos=0;
    for(int i=0;i<Xammount*Yammount;i++){
      
        
        ofVec2f p;
        p.set(xPos,yPos);
        
        xPos+=gutter.x;
        if(xPos>ofGetWidth()*2-gutter.x){
            xPos=0;
            yPos+=gutter.y;
        }
        
        ofRectangle rectangle;
        rectangle.set(p.x,p.y,gutter.x,gutter.y);
        
        rect r;
        rects.push_back(r);
        rects.back().position.set(p.x,p.y);
        rects.back().rectangle.set(rectangle);
        rects.back().color=ofColor(ofRandom(255),10);
    }
    
    sender.setup("10.0.1.7", 8000);
    //this is the IP address of your iOS/Android device and the port it should be
    //set to receive on
    
    receiver.setup(9000);
    /*this is the port your game will receive data on.
     For us this is the important one! Set your mobile device to send on this port.*/
    
    m.setAddress("/1");
    
    
    
    ofxOscMessage sendBack;
    sendBack.addFloatArg(wanderD);
    sendBack.setAddress("/1/rotary2");
    sender.sendMessage(sendBack);
    sendBack.setAddress("/updatedVals/wanderD");
    sender.sendMessage(sendBack);
    
    sendBack.clear();
    sendBack.addFloatArg(wanderR);
    sendBack.setAddress("/1/rotary3");
    sender.sendMessage(sendBack);
    sendBack.setAddress("/updatedVals/wanderR");
    sender.sendMessage(sendBack);

    
    sendBack.clear();
    sendBack.addFloatArg(change);
    sendBack.setAddress("/1/rotary4");
    sender.sendMessage(sendBack);
    sendBack.setAddress("/updatedVals/change");
    sender.sendMessage(sendBack);
    
    sendBack.clear();
    sendBack.addFloatArg(maxspeed);
    sendBack.setAddress("/1/rotary7");
    sender.sendMessage(sendBack);
    sendBack.setAddress("/updatedVals/speed");
    sender.sendMessage(sendBack);
    
    
    sendBack.clear();
    sendBack.addFloatArg(targetAgentsNum);
    sendBack.setAddress("/1/rotary5");
    sender.sendMessage(sendBack);
    
    sendBack.clear();
    sendBack.addFloatArg(targetPointSize);
    sendBack.setAddress("/1/rotary6");
    sender.sendMessage(sendBack);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    points.clear();
    sizes.clear();
    
    for(int i=0;i<renderdAgents;i++){
        agents[i]->update();
        points.push_back(agents[i]->getPosition());
        float size = ofRandom(pointsize, pointsize);
        sizes.push_back(ofVec3f(size));
    }
    int total = (int)points.size();
    vbo.setVertexData(&points[0], total, GL_STATIC_DRAW);
    vbo.setNormalData(&sizes[0], total, GL_STATIC_DRAW);
    // fill in the colors and vertices
    
    //vbo.setColorData(&sizes[0], 10000, GL_STATIC_DRAW);
    
    fbo.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofEnableAlphaBlending();
    ofSetColor(0,0,0,fadespeed);
    ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
    fbo.end();
    
    
    
    for(int i=0;i<renderdAgents;i++){
        agents[i]->setWanderForce(wanderforce);
        agents[i]->setSpinForce(rotationforce);
        agents[i]->setSeekForce(seekforce);
        
        if(agents[i]->getPosition().x>fbo.getWidth()||agents[i]->getPosition().x<-fbo.getWidth()/2 ) {
            //  cout<<"goback"<<agents[i]->getPosition().length()<<endl;
            agents[i]->setSeekForce(5);
        }
        
        if(agents[i]->getPosition().y>fbo.getHeight()||agents[i]->getPosition().y<-fbo.getHeight()/2 ) {
            //  cout<<"goback"<<agents[i]->getPosition().length()<<endl;
            agents[i]->setSeekForce(5);
        }
        
     
        
        
        agents[i]->wanderD=wanderD;
        agents[i]->wanderR=wanderR;
        agents[i]->change=change;
        agents[i]->maxspeed=maxspeed;
        
        
    }
    
    
    /*
    // cout<<renderdAgents<<endl;
    if(renderdAgents<targetAgentsNum){
        renderdAgents+=5;
    }
    
    // cout<<renderdAgents<<endl;
    if(renderdAgents>targetAgentsNum){
        renderdAgents-=5;
    }
    */
    
    //our simple while loop to make sure we get all of our messages
    while (receiver.hasWaitingMessages()) {
        
        //Get the message, which will hold all of our arguments inside of it.
        //It's a collection of data
        
        ofxOscMessage m;
        //Pass a reference to that message to the receiver
        //we set up above using the getNextMessage function in the OSC add on.
        
        receiver.getNextMessage(&m);
        
        //This will be the message we send back from our game
        //to our device letting it know what value we received
        //from it and displaying that back to us so we know what our
        //current game settings are at.
        
        ofxOscMessage sendBack;
        
        //Remember our address tags are unique.
        //We set up the /game tag as our root address and each "/" denotes a sub tag.
        //If these strings are a match, we know the message that came in is our
        //amplitude.
        
        if(m.getAddress() == "/1/rotary2")
        {
            
            //This is critical.
            //Each type must match if you want to be able to run your code.
            //We know the first argument in our array of messages
            //will be a float if the above if statement evaluates to true.
            
            wanderD = m.getArgAsFloat(0);
            
            //Now we are going to pack up a collection of data to send back to
            //our device.
            //sendBack is also a collection of data we
            //add arguments to.
            //Add the value we set for our amplitude to the message and move on.
            
            sendBack.addFloatArg(wanderD);
            sendBack.setAddress("/updatedVals/wanderD");
            sender.sendMessage(sendBack);
            
        }
        
        else if (m.getAddress() == "/1/rotary3")
        {
            
            wanderR = m.getArgAsFloat(0);
            
            //Now we are going to pack up a collection of data to send back to
            //our device.
            //sendBack is also a collection of data we
            //add arguments to.
            //Add the value we set for our amplitude to the message and move on.
            
            sendBack.addFloatArg(wanderR);
            sendBack.setAddress("/updatedVals/wanderR");
            sender.sendMessage(sendBack);

            
        }
        
        
        else if (m.getAddress() == "/1/rotary7")
        {
            
            maxspeed = m.getArgAsFloat(0);
            
            //Now we are going to pack up a collection of data to send back to
            //our device.
            //sendBack is also a collection of data we
            //add arguments to.
            //Add the value we set for our amplitude to the message and move on.
            
            sendBack.addFloatArg(maxspeed);
            sendBack.setAddress("/updatedVals/maxspeed");
            sender.sendMessage(sendBack);
            
            
        }
        
        else if (m.getAddress() == "/1/rotary4")
        {
            
            change = m.getArgAsFloat(0);
            
            //Now we are going to pack up a collection of data to send back to
            //our device.
            //sendBack is also a collection of data we
            //add arguments to.
            //Add the value we set for our amplitude to the message and move on.
            
            sendBack.addFloatArg(maxspeed);
            sendBack.setAddress("/updatedVals/change");
            sender.sendMessage(sendBack);
            
            
        }
        
        
        else if (m.getAddress() == "/1/push1")
        {
            
            addBranch();
            
        }
        
        else if (m.getAddress() == "/1/push2")
        {
            
            addAllBranches();
            
        }
        
        
        else if (m.getAddress() == "/1/push3")
        {
            cout<<"num bev"<<renderdAgents<<endl;

            if(renderdAgents>1000){
                renderdAgents-=1000;}
            else if (renderdAgents>100){
                renderdAgents-=10;
            }else if (renderdAgents>1){
            renderdAgents-=1;
            }
            sendBack.addFloatArg(renderdAgents);
            sendBack.setAddress("/updatedVals/totalagents");
            sender.sendMessage(sendBack);
            cout<<"num"<<renderdAgents<<endl;
            
        }
        
        
        
        // check for mouse moved message
        else if(m.getAddress() == "/1/rotary5"){
            int anz = m.getArgAsInt32(0);
            if(anz > totalNumAgents)anz=totalNumAgents;
            renderdAgents=anz;
            targetAgentsNum=anz;
            
            sendBack.addFloatArg(targetAgentsNum);
            sendBack.setAddress("/updatedVals/totalagents");
            sender.sendMessage(sendBack);
            
        }
        
        
        else if(m.getAddress() == "/1/rotary6"){
            int size = m.getArgAsInt32(0);
            if(size>200)size=200;
            targetPointSize=size;
            
            sendBack.addFloatArg(targetPointSize);
            sendBack.setAddress("/1/label8");
            sender.sendMessage(sendBack);
        }
        
    }
    
    
    
    shrink();
    
    
    ofPixels pixels;
    fbo.readToPixels(pixels);

    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(255);
    fbo.begin();
    
    glDepthMask(GL_FALSE);
    
    
    ofColor c;
    c.setHsb(0, 0, 200);
    ofSetColor(c,200);
    
    ofEnablePointSprites();
    camera.begin();
   // drawRects();
    shader.begin();
   // ofEnableBlendMode(OF_BLENDMODE_ADD);

    
    texture.bind();
    vbo.draw(GL_POINTS, 0, (int)points.size());
    texture.unbind();
    shader.end();

    camera.end();
    
    
    ofDisablePointSprites();
   // ofDisableBlendMode();
   
    
    fbo.end();
    
   // drawRects();

    
    camera.begin();
    
    fbo.draw(-fbo.getWidth()/2,-fbo.getHeight()/2);
   // fbo.draw(0,0);

    camera.end();
    
    
    // Off-Syphonscreen stuff...
    if(!bHide){
        gui.draw();
    }
  //  drawGutter();
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   /*
    switch (key) {
        case 'h':
            for(int i=0;i<agents.size();i++){
                agents[i]->goHome();
            }
            break;
            
            
        case 'c':
            for(int i=0;i<agents.size();i++){
                agents[i]->setSeekForce(ofRandom(0.3,0.5));
                agents[i]->setRepulsionForce(0.3);
                
            }
            break;
            
        case 'C':
            for(int i=0;i<agents.size();i++){
                // agents[i]->setSeekForce(0.3);
                // agents[i]->setRepulsionForce(ofRandom(0.5,0.9));
                
            }
            break;
            
            
            
            
        case 'r':
            for(int i=0;i<agents.size();i++){
                agents[i]->setRandomPosition();
            }
            break;
            
        case 'a':
            //for(int i=0;i<agents.size();i++){
            //  agents[i]->setRandomHomePosition();
            // }
            //renderdAgents+=100;
            addAgents(1000);
            
            
            
            // if(renderdAgents>agents.size())renderdAgents=agents.size();
            break;
            
            
        case'1':
            targetPointSize=200;
            break;
            
        case'2':
            targetPointSize=50;
            break;
            
        case'3':
            targetPointSize=20;
            break;
            
            
        case'4':
            targetPointSize=10;
            break;
            
        case'5':
            targetPointSize=5;
            break;
            
            
            
        default:
            break;
    }*/
    
}



void ofApp::addAgents(int num){
    targetAgentsNum=targetAgentsNum+num;
    if(targetAgentsNum>totalNumAgents)targetAgentsNum=totalNumAgents;
    cout<<"targetAgentsNum "<<targetAgentsNum<<" "<<totalNumAgents<<endl;
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    if(key == 'p'){
        bHide = !bHide;
        gui.saveToFile("settings.xml");
        
    }
    
    
    if(key == 'l'){
        renderdAgents++;
    }
    
    if(key=='b'){
        
        addBranch();
    }
    
    if(key=='B'){
        
        addAllBranches();
    }
    
    
    if(key == 'r'){
        removeAgents(100);
    
    }
    
}

void ofApp::addBranch(){
    if(renderdAgents<totalNumAgents){
        renderdAgents++;
        agents[renderdAgents-1]->setPosition(agents[renderdAgents-2]->getPosition());
        
        agents[renderdAgents-1]->setVelocity(agents[renderdAgents-2]->getVelocity());
    }
    ofxOscMessage sendBack;

    sendBack.addFloatArg(renderdAgents);
    sendBack.setAddress("/updatedVals/totalagents");
    sender.sendMessage(sendBack);
    
    
    sendBack.setAddress("/1/rotary5");
    sender.sendMessage(sendBack);
    
    
    
    
}

void ofApp::addAllBranches(){
    
    int amount=renderdAgents;
    
    for(int i=0;i<amount;i++){
        if(renderdAgents<totalNumAgents){
            renderdAgents++;
            agents[renderdAgents-1]->setPosition(agents[i]->getPosition());
            agents[renderdAgents-1]->setVelocity(agents[i]->getVelocity());
            
            
            

            }else{
                cout<<"MAX AGENTS"<<endl;
            }
        ofxOscMessage sendBack;
        sendBack.addFloatArg(renderdAgents);
        sendBack.setAddress("/updatedVals/totalagents");
        sender.sendMessage(sendBack);
        
        
        
        sendBack.setAddress("/1/rotary5");
        sender.sendMessage(sendBack);
    }
    
}


void ofApp::removeAgents(int _num){
    renderdAgents-=_num;
}


void ofApp::drawGutter(){
    ofPushStyle();
    ofSetColor(255, 0, 0);
    ofSetLineWidth(1);
    int Xammount=ofGetWidth()/gutter.x;
    int Yammount=ofGetHeight()/gutter.y;

    
    for(int i=0;i<Xammount;i++){
        ofDrawLine(i*gutter.x,0,i*gutter.x,ofGetHeight());
    }

    for(int i=0;i<Yammount;i++){
        ofDrawLine(0,i*gutter.y,ofGetWidth(),i*gutter.y);
    }
    ofPopStyle();
}

void ofApp::drawRects(){
    ofPushMatrix();
    ofTranslate(-fbo.getWidth()/2,-fbo.getHeight()/2);
    ofPushStyle();
    for(int i=0;i<rects.size();i++){
        ofSetColor(rects[i].color);
        ofDrawRectangle(rects[i].rectangle);
    }
    ofPopStyle();
    
    ofPopMatrix();
}


void ofApp::shrink(){
    int diff=targetPointSize-pointsize;
    if(diff>0)pointsize+=1;
    if(diff<0)pointsize-=1;
    //cout<<"t"<<targetPointSize<<" diff "<<diff<<endl;
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
