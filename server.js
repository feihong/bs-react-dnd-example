const express = require('express')
app = express()
app.use(express.static('.'))
const listener = app.listen(process.env.PORT || 8000, () => {
  console.log(`Your app is listening on http://localhost:${listener.address().port}`)
})
