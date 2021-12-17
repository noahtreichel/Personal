import React from 'react';
import { BrowserRouter as Router, Route } from 'react-router-dom';
import Image from 'react-image-resizer';
import stockImage from './img/stockImage.png';
import Home from './pages/Home';
import LoggedInHome from './pages/LoggedInHome';
import Quote from './pages/Quote';
import LoggedInQuote from './pages/LoggedInQuote';
import PriceHistory from './pages/PriceHistory';
import LoggedInPriceHistory from './pages/LoggedInPriceHistory';
import Login from './pages/Login';
import './App.css';


function App() {  
  return (
    <Router>
        <div className='App'>
          <header className='App-header'>
            <br></br>
            <Image
              src={stockImage}
              height={ 200 }
              width={ 300 } />             

            <Route exact path='/' component={Home} />
            <Route path='/logged_in' component={LoggedInHome} />
            <Route path='/quote' component={Quote} />
            <Route path='/logged_in_quote' component={LoggedInQuote} />
            <Route path='/price_history' component={PriceHistory} />
            <Route path='/logged_in_price_history' component={LoggedInPriceHistory} />
            <Route path='/login' component={Login} />
          </header>
        </div>
    </Router>
  );
}

export default App;
