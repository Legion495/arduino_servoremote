var gulp      = require('gulp');
var cssnano   = require('gulp-cssnano');
var gzip      = require('gulp-gzip');
var jade      = require('gulp-jade');
var rename    = require('gulp-rename');
var stylus    = require('gulp-stylus');
var uglify    = require('gulp-uglify');

var destination = "./dist/";

gulp.task("css", function() {
  return gulp.src(["gulp/css/*.css"])
    .pipe(cssnano())
    .pipe(rename({suffix: ".min"}))
    .pipe(gulp.dest("./dist/"))
    .pipe(gzip({gzipOptions: {level: 9}}))
    .pipe(gulp.dest("./arduino/data/"));
});

gulp.task("js", function() {
  return gulp.src(["gulp/js/*.js"])
    .pipe(uglify())
    .pipe(rename({suffix: ".min"}))
    .pipe(gulp.dest("./dist/"))
    .pipe(gzip({gzipOptions: {level: 9}}))
    .pipe(gulp.dest("./arduino/data/"));
});

gulp.task("jade", function() {
  return gulp.src(["gulp/jade/*.jade"])
    .pipe(jade())
    .pipe(gulp.dest("./dist/"))
    .pipe(gulp.dest("./arduino/data/"));
});

gulp.task("watch:css", ["css"], function() {
  return gulp.watch("gulp/css/*.css", ["css"]);
});

gulp.task("watch:jade", ["jade"], function() {
  return gulp.watch("gulp/jade/*.jade", ["jade"]);
});

gulp.task("watch:js", ["js"], function() {
  return gulp.watch("gulp/js/*.js", ["js"]);
});


gulp.task("build", ["css", "jade", "js"]);
gulp.task("dev", ["watch:jade", "watch:css", "watch:js"]);
