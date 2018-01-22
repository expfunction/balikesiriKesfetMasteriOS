//
//  agGL.m
//  balikesiriKesfet
//
//  Created by xloop on 26/10/2017.
//  Copyright © 2017 Xloop. All rights reserved.
//

#import "agGL.h"
#import "App.h"
#import "EngineBase.h"
#import "Logger.h"
#import "Reachability.h"
#import <string>
#import <malloc/malloc.h>
#import <AVFoundation/AVFoundation.h>
#import "SWRevealViewController.h"

@interface agGL () <CLLocationManagerDelegate, AVCaptureVideoDataOutputSampleBufferDelegate, SWRevealViewControllerDelegate> {
    Reachability *internetReachableFoo;
}

@property (nonatomic, strong) EAGLContext* context;
@property (nonatomic, strong) GLKBaseEffect* bEffect;

@end

//AllGlobals
#pragma mark - Custom macros
#define radToDeg(x) (180.0f/M_PI)*x
#define degToRad(x) (M_PI/180.0f)*x
#define kFilteringFactor 0.1

#pragma mark - Global Bools
bool checkFrameBuffer, drawTest, drawApp, glInitialized, iAvailable, locationInited, drawAppCalled;
#pragma mark - Global floats
float cPitch, cYaw, cRoll, initYaw, heading;
float rateSumX, rateSumY, rateSumZ;
#pragma mark - Global Strings
NSString *curLat, *curLng, *gyroStr, *accStr, *acStr, *apStr, *arStr;
#pragma mark - Global Location
CLLocation *currentLocation;
//static pinData *pinList=NULL;
static pinData* pinList = NULL;
#pragma mark - Globals
int pinCount = 0;
char* tmpString=NULL;
float motionLastYaw=0.0f;
NSMutableArray *constTextList;
@implementation agGL {
    
#pragma mark - AVFoundation Variables
    AVCaptureSession* captureSession;
    CVOpenGLESTextureCacheRef _videoTextureCache;
    CVOpenGLESTextureRef camTextureRefY;
    CVOpenGLESTextureRef camTextureRefUV;
    
#pragma mark - GL view
    GLKView* glView;
    
}

#pragma mark - ViewController Methods And Delegates
- (void)viewDidLoad {
    [super viewDidLoad];
    //Setup side menu
    [self.openMenuBut setTarget:self.revealViewController];
    [self.openMenuBut setAction:@selector(revealToggle:)];
    self.revealViewController.rearViewRevealWidth = 190;
    self.revealViewController.rearViewRevealOverdraw = 200;
    self.revealViewController.delegate = self;
    [self.navigationController.navigationBar addGestureRecognizer:self.revealViewController.panGestureRecognizer];
    [self.navigationController.navigationBar addGestureRecognizer:self.revealViewController.tapGestureRecognizer];
    
    glInitialized = false;
    checkFrameBuffer = false;
    drawTest = false;
    drawApp = true;
    iAvailable = false;
    locationInited = false;
    rateSumX = degToRad(90.0f);  rateSumY = 0.0f; rateSumZ = 0.0f;
    
    
    [self testInternetConnection];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    initYaw = 0.0f;
    
    //Init engine
    [self initTemplateAppWithGL];
    [self startCameraPreview];
    
    //Start location manager to get current location
    [self startCaptureLocation];
    
    //Start motion manager for camera orientation
    self.motionManager = [[CMMotionManager alloc] init];
    self.motionManager.deviceMotionUpdateInterval = 1.0f/30.0f;
    self.motionManager.accelerometerUpdateInterval = 1.0f/30.0f;
    self.motionManager.gyroUpdateInterval = 1.0f/30.0f;
    
    
    [self.motionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryCorrectedZVertical
                                                            toQueue:[NSOperationQueue currentQueue]
                                                        withHandler:^(CMDeviceMotion* motion, NSError* error){[self outputMotionData:motion];}];
    
    [self.motionManager startGyroUpdatesToQueue:[NSOperationQueue currentQueue]
                                    withHandler:^(CMGyroData* gyro, NSError *error) {
                                        [self outputGyroData:gyro];
                                    }];
    
    
    [self.motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue currentQueue] withHandler:^(CMAccelerometerData* acceleration, NSError *error) {
        [self outputAccelerationData:acceleration];
    }];
    
    //Tap handler for the pinclick
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapHandler:)];
    tap.numberOfTapsRequired = 1;
    [self.view addGestureRecognizer:tap];
    
    //Debug rect
    infoLabel = [[UILabel alloc]initWithFrame:CGRectMake(10, 500, 370, 100)];
    infoLabel.text = @"";
    [infoLabel setBackgroundColor:[UIColor colorWithRed:0.5f green:0.5f blue:0.5f alpha:1.0f]];
    [infoLabel setFont:[UIFont boldSystemFontOfSize:14]];
    [infoLabel setTextColor:[UIColor colorWithRed:1.0f green:1.0f blue:1.0f alpha:1.0f]];
    [infoLabel setNumberOfLines:5];
    [self.view addSubview:infoLabel];
    
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    printf("Memory warning!!!\n");
}

-(void) initTemplateAppWithGL {
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    glView = (GLKView*)self.view;
    glView.context = self.context;
    
    [EAGLContext setCurrentContext:self.context];
    
    self.bEffect = [[GLKBaseEffect alloc] init];
    //Generate framebuffer and bind to the view
    [((GLKView *) self.view) bindDrawable];
    
    printf("GLSL Version = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("GL Version = %s\n", glGetString(GL_VERSION));
    
    
    // Do any additional setup after loading the view.
    if(checkFrameBuffer) {
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOGI("\nNo framebuffer!!!\n");
        else
            LOGI("\nThere IS framebuffer!!!\n");
    }
    
    //Get resolution
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat screenWidth = screenRect.size.width ;
    CGFloat screenHeight = screenRect.size.height;
    
    
    //TEMPLATE APP
    templateApp.InitCamera(65.0f,1.0f,1000.0f,0.5f,true);
    templateApp.Init((int)screenWidth,(int)screenHeight);
    glInitialized = true;
}

bool pInited = false;
-(void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    drawAppCalled = false;
    
    if(drawApp && glInitialized) {
        templateApp.SetCameraRotation(cPitch, cYaw, cRoll);
        if(pInited) {
            if(pinCount>0 && pinList != NULL){
                for(int i=0; i<pinCount; i++){
                    pinList[i].text = (char*)[constTextList[i] cStringUsingEncoding:NSUTF8StringEncoding];
                    //LOGI("obj-c mainLoop pin[%d] posx: %.3f textaddress: %p text: %s\n",i,pinList[i].position.x,pinList[i].text,pinList[i].text);
                }
            }
            templateApp.Draw();
            drawAppCalled = true;
        }
    }
    
    
    ///////////////////////////
    //Framebuffer check
    if(checkFrameBuffer) {
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOGI("\nNo framebuffer!!!\n");
        else
            LOGI("\nThere IS framebuffer!!!\n");
    }
}

-(void)update {
    infoLabel.text = [NSString stringWithFormat:@"%@\n%@\n%@\n%@\n%@",gyroStr,accStr,acStr,apStr,arStr];
    //NSLog(@"Capture session running: %d",captureSession.running);
    //    [infoLabel setNumberOfLines:0]
    //    [infoLabel sizeToFit];
    
}

//Location Manager delegates
- (void)startCaptureLocation
{
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
    self.locationManager.desiredAccuracy = kCLLocationAccuracyBest;
    
    self.locationManager.headingFilter = kCLHeadingFilterNone;
    [self.locationManager startUpdatingLocation];
    [self.locationManager startUpdatingHeading];
    NSLog(@"Update location started...",nil);
    
}

- (void)stopCaptureLocation
{
    [self.locationManager stopUpdatingLocation];
    [self.locationManager stopUpdatingHeading];
    self.locationManager = nil;
    NSLog(@"Update location stopped...",nil);
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
    NSLog(@"Location Update Failed With Error: %@",error,nil);
}
- (void)locationManager:(CLLocationManager *)manager
    didUpdateToLocation:(CLLocation *)newLocation
           fromLocation:(CLLocation *)oldLocation
{
    currentLocation = newLocation;
    CLLocationDistance lDistance = [newLocation distanceFromLocation:oldLocation];
    if(!locationInited) {
        locationInited = true;
        curLat = [NSString stringWithFormat:@"%.8f",newLocation.coordinate.latitude];
        curLng = [NSString stringWithFormat:@"%.8f",newLocation.coordinate.longitude];
        [self updatePins];
        initYaw = degToRad(heading);
    }
    if(locationInited && lDistance > 100.0f){
        curLat = [NSString stringWithFormat:@"%.8f",newLocation.coordinate.latitude];
        curLng = [NSString stringWithFormat:@"%.8f",newLocation.coordinate.longitude];
        //[self updatePins];
        initYaw = degToRad(heading);
    }
}

- (void) tapHandler:(id)sender
{
    CGPoint location = [sender locationInView:self.view];
    CGFloat x = location.x;
    CGFloat y = location.y;
    
    
    if(glInitialized && pinCount > 0) {
        templateApp.ToucheBegan(x,y,1);
        templateApp.ToucheEnded(x,y,1);
        
        if(templateApp.GetSelectedPin() != NULL) {
            LOGI("\n\ntmp text: %s\n\n",templateApp.GetSelectedPin()->text);
            for(int i=0; i<pinCount; i++) {
                if(&pinList[i] == templateApp.GetSelectedPin()) {
                    pinList[i].borderColor = {1.0f, 0.0f, 0.0f};
                }
                else {
                    pinList[i].borderColor = {1.0f, 1.0f, 1.0f};
                }
                LOGI("pin[%d] reset, text: %s colB: %f\n",i, pinList[i].text, pinList[i].borderColor.y);
            }
            //templateApp.SetPinDatas(pinList, pinCount, 1.0f);
        }
        //free(templateApp.GetSelectedPin());
    }
}

- (void) locationManager:(CLLocationManager *)manager
        didUpdateHeading:(CLHeading *)newHeading {
    heading = newHeading.magneticHeading; //in degrees
    //cYaw = degToRad(heading);
}

//Angles from gyro update
struct holder{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};
holder lastUpdate;
holder lastDg;
-(void)outputGyroData:(CMGyroData*) gyro {
    double dgChange, newDg, x, y, z, motionInterval;
    motionInterval = 1.0f/30.0f;
    
    
    x = gyro.rotationRate.x;
    dgChange = radToDeg((0.5f * (x+lastUpdate.x))*motionInterval);
    lastUpdate.x = x;
    newDg = lastDg.x + dgChange;
    if(fabs(newDg) > 360.0f) {
        if(newDg < 0.0f) lastDg.x = newDg+360.0f; else lastDg.x = newDg - 360.0f;
    } else {
        lastDg.x = newDg;
    }
    
    y = gyro.rotationRate.y;
    dgChange = radToDeg((0.5f * (y+lastUpdate.y))*motionInterval);
    lastUpdate.y = y;
    newDg = lastDg.y + dgChange;
    if(fabs(newDg) > 360.0f) {
        if(newDg < 0.0f) lastDg.y = newDg+360.0f; else lastDg.y = newDg - 360.0f;
    } else {
        lastDg.y = newDg;
    }
    
    z = gyro.rotationRate.z;
    dgChange = radToDeg((0.5f * (z+lastUpdate.z))*motionInterval);
    lastUpdate.z = z;
    newDg = lastDg.z + dgChange;
    if(fabs(newDg) > 360.0f) {
        if(newDg < 0.0f) lastDg.z = newDg+360.0f; else lastDg.z = newDg - 360.0f;
    } else {
        lastDg.z = newDg;
    }
    //NSLog(@"gXang: %.2f gYang: %.2f gZang: %.2f",lastDg.x, lastDg.y, lastDg.z);
    gyroStr = [NSString stringWithFormat:@"Gyro gXang: %.2f gYang: %.2f gZang: %.2f",lastDg.x, lastDg.y, lastDg.z];
    cYaw = degToRad(-lastDg.y); cPitch = degToRad(-lastDg.x); cRoll = degToRad(lastDg.z);
}

-(void)outputAccelerationData:(CMAccelerometerData*) acceleration {
    accStr = [NSString stringWithFormat:@"Raw Acceleration x: %.2f y: %.2f z: %.2f",acceleration.acceleration.x, acceleration.acceleration.y, acceleration.acceleration.z];
    
    
}

struct dOrientation {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};
dOrientation angles;
-(void)outputMotionData:(CMDeviceMotion*) motion {
    CMQuaternion quat = motion.attitude.quaternion;
    
    //      CGFloat roll  = atan2(2*(quat.y*quat.w - quat.x*quat.z), 1 - 2*quat.y*quat.y - 2*quat.z*quat.z);
    //      CGFloat pitch = atan2(2*(quat.x*quat.w + quat.y*quat.z), 1 - 2*quat.x*quat.x - 2*quat.z*quat.z);
    //      CGFloat yaw   =  asin(2*(quat.x*quat.y + quat.w*quat.z));
    
    CGFloat pitch = atan2(2*(quat.y*quat.z + quat.w*quat.x), quat.w*quat.w - quat.x*quat.x - quat.y*quat.y + quat.z*quat.z);
    CGFloat yaw = asin(-2*(quat.x*quat.z - quat.w*quat.y));
    CGFloat roll = atan2(2*(quat.x*quat.y + quat.w*quat.z), 1 - 2*quat.y*quat.y - 2*quat.z*quat.z);
    
    
    if(pitch<0.0f) pitch = degToRad(360.0f) + pitch;
    if(roll<0.0f) roll = degToRad(360.0f) + roll;
    if(yaw<0.0f) yaw = degToRad(360.0f) + yaw;
    
    
    //cYaw = -roll;
    
    apStr = [NSString stringWithFormat:@"Attitude pitch: %.2f yaw: %.2f roll: %.2f",radToDeg(motion.attitude.pitch), radToDeg(motion.attitude.yaw), radToDeg(motion.attitude.roll)];
    arStr = [NSString stringWithFormat:@"Attitude rot rate x: %.2f y: %.2f z: %.2f",motion.rotationRate.x, motion.rotationRate.y, motion.rotationRate.z];
    acStr = [NSString stringWithFormat:@"Attitude acceleration x: %.2f y: %.2f z: %.2f",motion.userAcceleration.x, motion.userAcceleration.y, motion.userAcceleration.z];

}

#pragma mark UpdatePins
-(void) updatePins {
    pInited = false;
    NSString *generatedURL = [NSString stringWithFormat:@"http://app.balikesirikesfet.com/json_distance?lat=%@&lng=%@&dis=1",curLat,curLng];
    NSLog(@"generated: %@",generatedURL);
    NSURLRequest *request = [NSURLRequest requestWithURL:
                             [NSURL URLWithString:generatedURL]];
    NSURLSession *session = [NSURLSession sharedSession];
    NSURLSessionDataTask *dataTask = [session dataTaskWithRequest:request
                                                completionHandler:^(NSData *data, NSURLResponse *response, NSError *error)
                                      {
                                          NSString* newStr = [[NSString alloc] initWithData:data
                                                                                   encoding:NSUTF8StringEncoding];
                                          
                                          //delete html parts if exists
                                          NSArray * components = [newStr componentsSeparatedByString:@"<br />"];
                                          newStr = (NSString *)[components objectAtIndex:0];
                                          //NSLog(@"json part: %@",newStr);
                                          
                                          // do something with the data
                                          NSData *jsonData = [newStr dataUsingEncoding:NSUTF8StringEncoding];
                                          NSError *jsError;
                                          id jsonObj = [NSJSONSerialization JSONObjectWithData:jsonData options:0 error:&jsError];
                                          if (error) {
                                              NSLog(@"Error parsing JSON: %@", error);
                                          }
                                          else
                                          {
                                              //Re-init pins
                                              if ([jsonObj isKindOfClass:[NSArray class]])
                                              {
                                                  //NSLog(@"it is an array!");
                                                  NSArray *jsonArray = (NSArray *)jsonObj;
                                                  if(jsonArray.count>0 && glInitialized) {
                                                      pinCount = (int)jsonArray.count;
                                                      pinList = NULL;
                                                      pinList = (pinData*)malloc(sizeof(pinData)*(int)jsonArray.count);
                                                      constTextList = [[NSMutableArray alloc]init];
                                                      for(int cnt=0; cnt<jsonArray.count; cnt++){
                                                          
                                                          float pLat = [(jsonArray[cnt][@"lat"]) floatValue];
                                                          float pLng = [(jsonArray[cnt][@"lng"]) floatValue];
                                                          CLLocation* tmpPinLoc = [[CLLocation alloc] initWithLatitude:pLat longitude:pLng];
                                                          
                                                          pLat = (tmpPinLoc.coordinate.latitude - currentLocation.coordinate.latitude)*100000;
                                                          pLng = (tmpPinLoc.coordinate.longitude - currentLocation.coordinate.longitude)*100000;
                                                          
                                                          [constTextList addObject:jsonArray[cnt][@"title"]];
                                                          pinList[cnt].id = cnt;
                                                          pinList[cnt].position = {-pLat, 0.0f, pLng};
                                                          pinList[cnt].text = (char*)[constTextList[cnt] cStringUsingEncoding:NSUTF8StringEncoding];
                                                          pinList[cnt].size = 4.0f;
                                                          pinList[cnt].fontSize = 0.65f;
                                                          pinList[cnt].color = {0.0f, 1.0f, 0.0f, 1.0f};
                                                          pinList[cnt].borderColor = {1.0f, 1.0f, 1.0f, 1.0f};
                                                          
                                                          LOGI("obj-c pin init [%d] text: %s address: %p\n", cnt, pinList[cnt].text, pinList[cnt].text);
                                                      }
                                                      LOGI("\n\n");
                                                      templateApp.SetPinDatas(pinList,pinCount,1.0f);
                                                      pInited = true;
                                                  } else {
                                                      pinCount = 0;
                                                  }
                                              }
                                              else {
                                                  NSLog(@"it is a dictionary");
                                                  NSDictionary *jsonDictionary = (NSDictionary *)jsonObj;
                                                  NSLog(@"jsonDictionary - %@",jsonDictionary);
                                              }
                                          }
                                      }];
    [dataTask resume];
}

-(void) startCameraPreview {
    //-- Create CVOpenGLESTextureCacheRef for optimal CVImageBufferRef to GLES texture conversion.
#if COREVIDEO_USE_EAGLCONTEXT_CLASS_IN_API
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, _context, NULL, &_videoTextureCache);
#else
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, (__bridge void *)_context, NULL, &_videoTextureCache);
#endif
    if (err)
    {
        NSLog(@"1-Error at CVOpenGLESTextureCacheCreate %d", err);
        return;
    }
    //Video Device
    captureSession = [AVCaptureSession new];
    captureSession = [[AVCaptureSession alloc] init];
    [captureSession beginConfiguration];
    AVCaptureDevice* videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    if(videoDevice == nil) {
        assert(0);
        NSLog(@"video device error...");
    }
    
    //Add device to session
    NSError* error;
    AVCaptureInput* input = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error: &error];
    if(error) {
        assert(0);
        NSLog(@"video device session error...");
    }
    [captureSession addInput:input];
    //preview layer
    
    
    //-- Create the output for the capture session.
    AVCaptureVideoDataOutput * dataOutput = [[AVCaptureVideoDataOutput alloc] init];
    [dataOutput setAlwaysDiscardsLateVideoFrames:YES]; // Probably want to set this to NO when recording
    
    //-- Set to YUV420.
    [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange]
                                                             forKey:(id)kCVPixelBufferPixelFormatTypeKey]]; // Necessary for manual preview
    
    // Set dispatch to be on the main thread so OpenGL can do things with the data
    [dataOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    
    [captureSession addOutput:dataOutput];
    [captureSession commitConfiguration];
    
    [captureSession startRunning];
}
//AV Outputdelegate
- (void)captureOutput:(AVCaptureOutput *)captureOutput didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer fromConnection:(AVCaptureConnection *)connection {
    //NSLog(@"capture output works...");
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    GLsizei width = (GLsizei)CVPixelBufferGetWidth(pixelBuffer);
    GLsizei height = (GLsizei)CVPixelBufferGetHeight(pixelBuffer);
    
    if (!_videoTextureCache)
    {
        NSLog(@"No video texture cache");
        return;
    }
    
    //[self cleanUpTextures];
    if(camTextureRefY){
        CFRelease(camTextureRefY);
    }
    if(camTextureRefUV){
        CFRelease(camTextureRefUV);
    }
    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
    
    // CVOpenGLESTextureCacheCreateTextureFromImage will create GLES texture
    // optimally from CVImageBufferRef.
    
    // Y-plane
    glActiveTexture(GL_TEXTURE0);
    CVReturn err;
    err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                       _videoTextureCache,
                                                       pixelBuffer,
                                                       NULL,
                                                       GL_TEXTURE_2D,
                                                       GL_LUMINANCE,
                                                       width,
                                                       height,
                                                       GL_LUMINANCE,
                                                       GL_UNSIGNED_BYTE,
                                                       0,
                                                       &camTextureRefY);
    if (err)
    {
        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
    }
    GLuint textureId =CVOpenGLESTextureGetName(camTextureRefY);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // UV-plane
    glActiveTexture(GL_TEXTURE1);
    err = 0;
    err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                       _videoTextureCache,
                                                       pixelBuffer,
                                                       NULL,
                                                       GL_TEXTURE_2D,
                                                       GL_RG_EXT,
                                                       width/2,
                                                       height/2,
                                                       GL_RG_EXT,
                                                       GL_UNSIGNED_BYTE,
                                                       1,
                                                       &camTextureRefUV);
    if (err)
    {
        NSLog(@"Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", err);
    }
    GLuint textureIdUV =CVOpenGLESTextureGetName(camTextureRefUV);
    glBindTexture(GL_TEXTURE_2D, textureIdUV);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    templateApp.BindCameraTexture(textureId,textureIdUV);
}

-(void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    glInitialized = false; drawApp = false;
    [self.motionManager stopDeviceMotionUpdates];
    [self.motionManager stopGyroUpdates];
    [self.motionManager stopMagnetometerUpdates];
    [self.motionManager stopAccelerometerUpdates];
    [self stopCaptureLocation];
    templateApp.Exit();
    //Dealloc
    free(pinList);
    pinCount = 0;
}

//SWRevealController Delegate
- (void)revealController:(SWRevealViewController *)revealController didMoveToPosition:(FrontViewPosition)position {
    long tagId = 4207868622;
    if(position == FrontViewPositionLeft) {
        UIView* lock = [self.view viewWithTag:tagId];
        [lock setAlpha:0.333];
        [UIView animateWithDuration:0.5 animations:^{
            lock.alpha = 0.0;
        } completion:^(BOOL finished) {
            [lock removeFromSuperview];
        }];
    }
    if(position == FrontViewPositionRight) {
        UIView* lock = [[UIView alloc] initWithFrame:self.view.bounds];
        lock.tag = tagId;
        lock.autoresizingMask = (UIViewAutoresizingFlexibleWidth |
                                 UIViewAutoresizingFlexibleHeight);
        [lock setAlpha:0.333];
        [lock setBackgroundColor:[UIColor blackColor]];
        UITapGestureRecognizer* tapGst = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapGesture:)];
        [lock addGestureRecognizer:tapGst];
        [UIView animateWithDuration:0.5 animations:^{
            lock.alpha = 0.333;
        }];
    }
}
-(void) tapGesture: (id)sender {
    [self.revealViewController revealToggle:sender];
}


// Checks if we have an internet connection or not
- (void)testInternetConnection
{
    LOGI("testing connection\n");
    internetReachableFoo = [Reachability reachabilityWithHostname:@"www.google.com"];
    
    // Internet is reachable
    internetReachableFoo.reachableBlock = ^(Reachability*reach)
    {
        // Update the UI on the main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"Yayyy, we have the interwebs!");
            iAvailable = true;
        });
    };
    
    // Internet is not reachable
    internetReachableFoo.unreachableBlock = ^(Reachability*reach)
    {
        // Update the UI on the main thread
        dispatch_async(dispatch_get_main_queue(), ^{
            NSLog(@"Someone broke the internet :(");
            iAvailable = false;
        });
    };
    
    [internetReachableFoo startNotifier];
}

@end

