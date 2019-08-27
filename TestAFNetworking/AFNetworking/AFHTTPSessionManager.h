//
//  AFHTTPSessionManager.h
//  TestAFNetworking
//
//  Created by tongleiming on 2019/8/26.
//  Copyright © 2019 tongleiming. All rights reserved.
//

#import "TLMAFURLSessionManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface AFHTTPSessionManager : TLMAFURLSessionManager <NSSecureCoding, NSCopying>

// Base url，为了更方便地使用<相对路径>的request。
@property (readonly, nonatomic, strong, nullable) NSURL *baseURL;

@property (nonatomic, strong) AFHTTPRequestSerializer <AFURLRequestSerialization> * requestSerializer;

// 内容协商的基础上处理response
#warning 重写了父类的属性，
#warning 在.m文件中，写了"@dynamic responseSerializer;"
@property (nonatomic, strong) AFHTTPResponseSerializer <AFURLResponseSerialization> * responseSerializer;

///-------------------------------
/// @name 管理安全策略
///-------------------------------

#warning 子类重写属性，需要注意什么?
#warning 在.m中，写了“@dynamic securityPolicy;”
// 默认是采用default，只校验服务器证书。公钥验证和证书验证好像只能用在AFURLSessionManager中的securityPolicy上。
@property (nonatomic, strong) AFSecurityPolicy *securityPolicy;

///---------------------
/// @name 初始化
///---------------------

// 创建并返回一个AFHTTPSessionManager
+ (instancetype)manager;

// base URL for the HTTP client.
// url是base url
- (instancetype)initWithBaseURL:(nullable NSURL *)url;

// 是指定初始化函数
- (instancetype)initWithBaseURL:(nullable NSURL *)url
           sessionConfiguration:(nullable NSURLSessionConfiguration *)configuration NS_DESIGNATED_INITIALIZER;

///---------------------------
/// @name 创建HTTP请求
///---------------------------

// 创建一个GET请求方式的NSURLSessionDataTask，并开始执行
- (nullable NSURLSessionDataTask *)GET:(NSString *)URLString
                            parameters:(nullable id)parameters
                              progress:(nullable void (^)(NSProgress *downloadProgress))downloadProgress
                               success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                               failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure DEPRECATED_ATTRIBUTE;

@end

NS_ASSUME_NONNULL_END
