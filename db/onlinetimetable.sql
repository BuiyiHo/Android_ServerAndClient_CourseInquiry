/*
Navicat MySQL Data Transfer

Source Server         : 本机MySQL
Source Server Version : 80029
Source Host           : localhost:3306
Source Database       : onlinetimetable

Target Server Type    : MYSQL
Target Server Version : 80029
File Encoding         : 65001

Date: 2022-11-27 16:34:15
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for course
-- ----------------------------
DROP TABLE IF EXISTS `course`;
CREATE TABLE `course` (
  `id` int NOT NULL AUTO_INCREMENT,
  `code` varchar(255) DEFAULT NULL,
  `title` varchar(255) DEFAULT NULL,
  `section` varchar(255) DEFAULT NULL,
  `time` datetime DEFAULT NULL,
  `classroom` varchar(255) DEFAULT NULL,
  `instructor` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of course
-- ----------------------------
INSERT INTO `course` VALUES ('1', '1001', 'English', '2', '2022-11-26 23:17:03', 'class 2', 'kangkang');
INSERT INTO `course` VALUES ('2', '1002', 'French', '2', '2022-11-12 23:18:10', 'class 2', 'pipi');
INSERT INTO `course` VALUES ('3', '1003', 'japanese', '1', '2022-11-04 23:18:43', 'class 3', 'mike');

-- ----------------------------
-- Table structure for log
-- ----------------------------
DROP TABLE IF EXISTS `log`;
CREATE TABLE `log` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(255) DEFAULT NULL,
  `connecttime` datetime DEFAULT NULL,
  `ip` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of log
-- ----------------------------
INSERT INTO `log` VALUES ('15', 'admin', '2022-11-27 13:30:30', '192.0.23.249');
INSERT INTO `log` VALUES ('16', 'student1', '2022-11-27 13:30:45', '192.0.23.249');
INSERT INTO `log` VALUES ('17', 'student2', '2022-11-27 13:31:04', '192.0.23.249');
INSERT INTO `log` VALUES ('18', 'aaa', '2022-11-27 13:34:07', '192.0.23.249');
INSERT INTO `log` VALUES ('19', 'student2', '2022-11-27 13:34:55', '192.0.23.249');

-- ----------------------------
-- Table structure for user
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` int NOT NULL AUTO_INCREMENT,
  `username` varchar(255) NOT NULL,
  `password` varchar(255) DEFAULT NULL,
  `role` int DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- ----------------------------
-- Records of user
-- ----------------------------
INSERT INTO `user` VALUES ('1', 'student1', '123456', '1');
INSERT INTO `user` VALUES ('2', 'student2', '123456789', '1');
INSERT INTO `user` VALUES ('3', 'admin', '123456', '2');
