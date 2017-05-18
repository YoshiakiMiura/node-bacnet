/* globals it, describe */

const should = require('should')
const bacnet = require('../bacnet.js')

describe('bacnet enum conversions', () => {
  describe('objectTypeToString', () => {
    it('converts 8 to "device"', () => {
      bacnet.objectTypeToString(8).should.equal('device')
    })
    it('converts "device" to "device"', () => {
      bacnet.objectTypeToString('device').should.equal('device')
    })
    it('errors on "foo"', () => {
      should.throws(() => {
        bacnet.objectTypeToString('foo')
      }, /foo/, 'no error was thrown for invalid object type "foo"')
    })
    it('errors on -1', () => {
      should.throws(() => {
        bacnet.objectTypeToString(-1)
      }, /-1/, 'no error was thrown for invalid object type "foo"')
    })
  })
  describe('objectTypeToNumber', () => {
    it('converts 8 to 8', () => {
      bacnet.objectTypeToNumber(8).should.equal(8)
    })
    it('converts "device" to 8', () => {
      bacnet.objectTypeToNumber('device').should.equal(8)
    })
    it('errors on "foo"', () => {
      should.throws(() => {
        bacnet.objectTypeToNumber('foo')
      }, /foo/, 'no error was thrown for invalid object type "foo"')
    })
    it('errors on -1', () => {
      should.throws(() => {
        bacnet.objectTypeToNumber(-1)
      }, /-1/, 'no error was thrown for invalid object type "foo"')
    })
  })
  describe('propertyKeyToString', () => {
    it('converts 76 to "object-list"', () => {
      bacnet.propertyKeyToString(76).should.equal('object-list')
    })
    it('converts "object-list" to "object-list"', () => {
      bacnet.propertyKeyToString('object-list').should.equal('object-list')
    })
    it('errors on "foo"', () => {
      should.throws(() => {
        bacnet.propertyKeyToString('foo')
      }, /foo/, 'no error was thrown for invalid property key "foo"')
    })
    it('errors on -1', () => {
      should.throws(() => {
        bacnet.propertyKeyToString(-1)
      }, /-1/, 'no error was thrown for invalid property key -1')
    })
    it('errors on 4194304', () => {
      should.throws(() => {
        bacnet.propertyKeyToString(4194304)
      }, /4194304/, 'no error was thrown for invalid property key 4194304')
    })
  })
  describe('propertyKeyToNumber', () => {
    it('doesnt change 76', () => {
      bacnet.propertyKeyToNumber(76).should.equal(76)
    })
    it('converts "object-list" to 76', () => {
      bacnet.propertyKeyToNumber('object-list').should.equal(76)
    })
    it('doesnt change proprietary property keys', () => {
      bacnet.propertyKeyToNumber(4194303).should.equal(4194303)
    })
    it('errors on "foo"', () => {
      should.throws(() => {
        bacnet.propertyKeyToNumber('foo')
      }, /foo/, 'no error was thrown for invalid property key "foo"')
    })
    it('errors on -1', () => {
      should.throws(() => {
        bacnet.propertyKeyToNumber(-1)
      }, /-1/, 'no error was thrown for invalid property key -1')
    })
    it('errors on 4194304', () => {
      should.throws(() => {
        bacnet.propertyKeyToNumber(4194304)
      }, /4194304/, 'no error was thrown for invalid property key 4194304')
    })
  })

  describe('applicationTagToString', () => {
    it('converts 0 to "Null"', () => {
      bacnet.applicationTagToString(0).should.equal('Null')
    })
    it('converts "Null" to "Null"', () => {
      bacnet.applicationTagToString('Null').should.equal('Null')
    })
    it('errors on "foo"', () => {
      should.throws(() => {
        bacnet.applicationTagToString('foo')
      }, /foo/, 'no error was thrown for invalid application tag "foo"')
    })
    it('errors on -1', () => {
      should.throws(() => {
        bacnet.applicationTagToString(-1)
      }, /-1/, 'no error was thrown for invalid application tag -1')
    })
    it('errors on 17', () => {
      should.throws(() => {
        bacnet.applicationTagToString(17)
      }, /17/, 'no error was thrown for invalid application tag 17')
    })
  })
  describe('applicationTagToNumber', () => {
    it('doesnt change 0', () => {
      bacnet.applicationTagToNumber(0).should.equal(0)
    })
    it('converts "Null" to 0', () => {
      bacnet.applicationTagToNumber('Null').should.equal(0)
    })
    it('doesnt change proprietary application tags', () => {
      bacnet.applicationTagToNumber(16).should.equal(16)
    })
    it('errors on "foo"', () => {
      should.throws(() => {
        bacnet.applicationTagToNumber('foo')
      }, /foo/, 'no error was thrown for invalid application tag "foo"')
    })
    it('errors on -1', () => {
      should.throws(() => {
        bacnet.applicationTagToNumber(-1)
      }, /-1/, 'no error was thrown for invalid application tag -1')
    })
    it('errors on 17', () => {
      should.throws(() => {
        bacnet.applicationTagToNumber(17)
      }, /17/, 'no error was thrown for invalid application tag 17')
    })
  })
})
