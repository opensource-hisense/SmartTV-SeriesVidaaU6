'use strict';
const PromiseProvider = require('../../lib/promise_provider');
const assert = require('assert');
const mongoose = require('../../');

describe('promises docs', function () {
  let Band;
  let db;

  before(function (done) {
    db = mongoose.createConnection('mongodb://localhost:27017/mongoose_test');

    Band = db.model('band-promises', {name: String, members: [String]});

    done();
  });

  beforeEach(function (done) {
    Band.deleteMany({}, done);
  });

  after(async function () {
    mongoose.Promise = global.Promise;

    await db.close();
  });

  /**
   * Mongoose async operations, like `.save()` and queries, return thenables.
   * This means that you can do things like `MyModel.findOne({}).then()` and
   * `await MyModel.findOne({}).exec()` if you're using
   * [async/await](http://thecodebarbarian.com/80-20-guide-to-async-await-in-node.js.html).
   * 
   * You can find the return type of specific operations [in the api docs](https://mongoosejs.com/docs/api.html)
   * You can also read more about [promises in Mongoose](https://masteringjs.io/tutorials/mongoose/promise).
   */
  it('Built-in Promises', function (done) {
    const gnr = new Band({
      name: "Guns N' Roses",
      members: ['Axl', 'Slash']
    });

    const promise = gnr.save();
    assert.ok(promise instanceof Promise);

    promise.then(function (doc) {
      assert.equal(doc.name, "Guns N' Roses");
      // acquit:ignore:start
      done();
      // acquit:ignore:end
    });
  });

  /**
   * [Mongoose queries](http://mongoosejs.com/docs/queries.html) are **not** promises. They have a `.then()`
   * function for [co](https://www.npmjs.com/package/co) and async/await as
   * a convenience. If you need
   * a fully-fledged promise, use the `.exec()` function.
   */
  it('Queries are not promises', function (done) {
    const query = Band.findOne({name: "Guns N' Roses"});
    assert.ok(!(query instanceof Promise));

    // acquit:ignore:start
    let outstanding = 2;
    // acquit:ignore:end

    // A query is not a fully-fledged promise, but it does have a `.then()`.
    query.then(function(doc) {
      // use doc
      // acquit:ignore:start
      assert.ok(!doc);
      --outstanding || done();
      // acquit:ignore:end
    });

    // `.exec()` gives you a fully-fledged promise
    const promise = Band.findOne({name: "Guns N' Roses"}).exec();
    assert.ok(promise instanceof Promise);

    promise.then(function (doc) {
      // use doc
      // acquit:ignore:start
      assert.ok(!doc);
      --outstanding || done();
      // acquit:ignore:end
    });
  });

  /**
   * Although queries are not promises, queries are [thenables](https://promisesaplus.com/#terminology).
   * That means they have a `.then()` function, so you can use queries as promises with either
   * promise chaining or [async await](https://asyncawait.net)
   */
  it('Queries are thenable', function (done) {
    Band.findOne({name: "Guns N' Roses"}).then(function(doc) {
      // use doc
      // acquit:ignore:start
      assert.ok(!doc);
      done();
      // acquit:ignore:end
    });
  });

  /**
   * There are two alternatives for using `await` with queries:
   * 
   * - `await Band.findOne();`
   * - `await Band.findOne().exec();`
   * 
   * As far as functionality is concerned, these two are equivalent.
   * However, we recommend using `.exec()` because that gives you
   * better stack traces.
   */
  it('Should You Use `exec()` With `await`?', async function() {
    const doc = await Band.findOne({ name: "Guns N' Roses" }); // works
    // acquit:ignore:start
    assert.ok(!doc);
    // acquit:ignore:end

    const badId = 'this is not a valid id';
    try {
      await Band.findOne({ _id: badId });
    } catch (err) {
      // Without `exec()`, the stack trace does **not** include the
      // calling code. Below is the stack trace:
      //
      // CastError: Cast to ObjectId failed for value "this is not a valid id" at path "_id" for model "band-promises"
      //   at new CastError (/app/node_modules/mongoose/lib/error/cast.js:29:11)
      //   at model.Query.exec (/app/node_modules/mongoose/lib/query.js:4331:21)
      //   at model.Query.Query.then (/app/node_modules/mongoose/lib/query.js:4423:15)
      //   at process._tickCallback (internal/process/next_tick.js:68:7)
      err.stack;
      // acquit:ignore:start
      assert.ok(!err.stack.includes('promises.test.js'));
      // acquit:ignore:end
    }

    try {
      await Band.findOne({ _id: badId }).exec();
    } catch (err) {
      // With `exec()`, the stack trace includes where in your code you
      // called `exec()`. Below is the stack trace:
      //
      // CastError: Cast to ObjectId failed for value "this is not a valid id" at path "_id" for model "band-promises"
      //   at new CastError (/app/node_modules/mongoose/lib/error/cast.js:29:11)
      //   at model.Query.exec (/app/node_modules/mongoose/lib/query.js:4331:21)
      //   at Context.<anonymous> (/app/test/index.test.js:138:42)
      //   at process._tickCallback (internal/process/next_tick.js:68:7)
      err.stack;
      // acquit:ignore:start
      assert.ok(err.stack.includes('promises.test.js'));
      // acquit:ignore:end
    }
  });
  
  /**
   * If you're an advanced user, you may want to plug in your own promise
   * library like [bluebird](https://www.npmjs.com/package/bluebird). Just set
   * `mongoose.Promise` to your favorite
   * ES6-style promise constructor and mongoose will use it.
   */
  it('Plugging in your own Promises Library', function (done) {
    // acquit:ignore:start
    if (!global.Promise) {
      return done();
    }
    // acquit:ignore:end
    // Use bluebird
    mongoose.Promise = require('bluebird');
    const bluebirdPromise = Band.findOne({name: "Guns N' Roses"}).exec();
    assert.equal(bluebirdPromise.constructor, require('bluebird'));

    // Use q. Note that you **must** use `require('q').Promise`.
    mongoose.Promise = require('q').Promise;
    const qPromise = Band.findOne({name: "Guns N' Roses"}).exec();
    assert.ok(qPromise instanceof require('q').makePromise);

    // acquit:ignore:start
    // Wait for promises
    bluebirdPromise.then(qPromise).then(function () {
      done();
    });
    // acquit:ignore:end
  });
});
