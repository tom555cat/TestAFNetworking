//
//  TLMAFURLSessionManager.h
//  TestAFNetworking
//
//  Created by tongleiming on 2019/8/2.
//  Copyright © 2019 tongleiming. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AFSecurityPolicy.h"

NS_ASSUME_NONNULL_BEGIN

@interface TLMAFURLSessionManager : NSObject <NSURLSessionDelegate, NSURLSessionTaskDelegate, NSURLSessionDataDelegate, NSURLSessionDownloadDelegate>

@property (readonly, nonatomic, strong) NSURLSession *session;

#warning operationQueue是如何使用的
@property (readonly, nonatomic, strong) NSOperationQueue *operationQueue;

// 这个一个遵守了策略模式的解析器，可以解析各种content-type
@property (nonatomic, strong) id <AFURLResponseSerialization> responseSerializer;

#warning 这个需要单独看
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

#warning 这个需要单独看
@property (readwrite, nonatomic, strong) AFNetworkReachabilityManager *reachabilityManager;

// 三种类型任务存放的数组，data,upload,download，基类是NSURLSessionTask
@property (readonly, nonatomic, strong) NSArray <NSURLSessionTask *> *task;

// data任务又单独分出一个数组
@property (readonly, nonatomic, strong) NSArray <NSURLSessionDataTask *> *dataTasks;

// 上传任务单独分出一个数组
@property (readonly, nonatomic, strong) NSArray <NSURLSessionUploadTask *> *uploadTasks;

// 下载任务单独分出一个数组
@property (readonly, nonatomic, strong) NSArray <NSURLSessionDownloadTask *> *downloadTasks;

///-----------------------------
/// @name 处理回调的队列
///-----------------------------

// 为执行'completionBlock'的队列，如果为NULL，则使用主队列
@property (nonatomic, strong, nullable) dispatch_queue_t completionQueue;

#warning 看这个如何使用的
// 为执行'completionBlock'的group，如果为NULL，则使用私有的group
@property (nonatomic, strong, nullable) dispatch_group_t completionGroup;

///-----------------------------
/// @name 系统bug解决working around
///-----------------------------

#warning 这个是为了解决bug而设置的
@property (nonatomic, assign) BOOL attemptsToRecreateUploadTasksForBackgroundSessions;

///-----------------------------
/// @name 初始化
///-----------------------------

- (instancetype)initWithSessionConfiguration:(nullable NSURLSessionConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

// invalidate session，顺便传递两个参数是否决定要取消任务和resetSession.
- (void)invalidateSessionCancelingTasks:(BOOL)cancelPendingTasks resetSession:(BOOL)resetSession;

///-----------------------------
/// @name 创建Data Tasks
///-----------------------------

- (NSURLSessionDataTask *)dataTaskWithRequest:(NSURLRequest *)request
                                :(nullable void(^)(NSProgress *uploadProgress))uploadProgressBlockuploadProgress
                             downloadProgress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                            completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject, NSError * _Nullable error))completionHandler;

///-----------------------------
/// @name 创建Upload Tasks
///-----------------------------

// 上传一个本地文件
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromFile:(NSURL *)fileURL
                                         progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                                completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject, NSError  * _Nullable error))completionHandler;

// 以HTTP Body的形式上传文件
- (NSURLSessionUploadTask *)uploadTaskWithRequest:(NSURLRequest *)request
                                         fromData:(nullable NSData *)bodyData
                                         progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                                completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject, NSError * _Nullable error))completionHandler;

// creates an `NSURLSessionUploadTask` with the specified streaming request.
- (NSURLSessionUploadTask *)uploadTaskWithStreamedRequest:(NSURLRequest *)request
                                                 progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgressBlock
                                        completionHandler:(nullable void (^)(NSURLResponse *response, id _Nullable responseObject, NSError * _Nullable error))completionHandler;
#warning 流形式的request是什么request?
#warning 从本地文件和http body上传文件最终调用的是一个方法吗?

///-----------------------------
/// @name 创建Download Tasks
///-----------------------------

// 下载任务
// 在Background Session中的创建下载任务，则当app terminated状态时block都会丢失。Backgroun sessions更推荐使用"-setDownloadTaskDidFinishDownloadingBlock:"去设置保存文件，而不是这个方法中设置下载地址。
#warning 可以测试一下后台下载通过这种方式丢失保存路径的情况
- (NSURLSessionDownloadTask *)downloadTaskWithRequest:(NSURLRequest *)request
                                             progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                                          destination:(nullable NSURL * (^)(NSURL *targetPath, NSURLResponse *response))destination
                                    completionHandler:(nullable void (^)(NSURLResponse *response, NSURL * _Nullable filePath, NSError * _Nullable error))completionHandler;

#warning resumeData是断点下载么？
- (NSURLSessionDownloadTask *)downloadTaskWithResumeData:(NSData *)resumeData
                                                progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgressBlock
                                             destination:(nullable NSURL * (^)(NSURL *targetPath, NSURLResponse *response))destination
                                       completionHandler:(nullable void (^)(NSURLResponse *response, NSURL * _Nullable filePath, NSError * _Nullable error))completionHandler;

///---------------------------------
/// @name 获取task的NSProgress
///---------------------------------

- (nullable NSProgress *)uploadProgressForTask:(NSURLSessionTask *)task;
- (nullable NSProgress *)downloadProgressForTask:(NSURLSessionTask *)task;

///-----------------------------------------
/// @name 设置Session Delegate中需要的回调
///-----------------------------------------

- (void)setSessionDidBecomeInvalidBlock:(nullable void (^)(NSURLSession *session, NSError *error))block;

// 设置处理重定向response的block
- (void)setTaskWillPerformHTTPRedirectionBlock:(nullable NSURLRequest * _Nullable (^)(NSURLSession *session, NSURLSessionTask *task, NSURLResponse *response, NSURLRequest *request))block;

///--------------------------------------
/// @name 设置Task Delegate中需要的回调
///--------------------------------------

///-------------------------------------------
/// @name 设置Data Task Delegate中需要的回调
///-------------------------------------------

///-----------------------------------------------
/// @name 设置Download Task Delegate中需要的回调
///-----------------------------------------------

@end

///--------------------
/// @name 通知
///--------------------

FOUNDATION_EXPORT NSString * const AFNetworkingTaskDidResumeNotification;

NS_ASSUME_NONNULL_END
