import React, { useState, useEffect } from 'react';
import { Navbar, Nav, NavLink, NavItem } from 'reactstrap';
import { useParams } from 'react-router-dom';
import { AgGridReact } from 'ag-grid-react';
import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham.css'; 


function LoggedInPriceHistory(props) {
  const [rowData, setRowData] = useState([]);

  const columns = [
    { headerName: 'Name', field: 'name' },
    { headerName: 'Symbol', field: 'symbol' },
    { headerName: 'Industry', field: 'industry', filter: true },
    { headerName: 'Open', field: 'open' },
    { headerName: 'High', field: 'high' },
    { headerName: 'Low', field: 'low' },
    { headerName: 'Close', field: 'close' },
    { headerName: 'Volumes', field: 'volumes' }
  ];

  let {symbol} = useParams();
  const [rowSymbol, setRowSymbol] = useState(symbol);
  let url = `http://131.181.190.87:3000/stocks/${rowSymbol}`;

  useEffect(() => {
    fetch(url)
    .then((response) => response.json())
    .then(stocks => { setRowData([stocks])
    });
  }, [symbol, url]);


  return (
    <div className='App-header'>
      <div>
        <Navbar expand='md'>
          <Nav className='Navbar' navbar>            
            <NavItem className='line'>
              <NavLink style={{ color:'white' }} href='/logged_in'>Home</NavLink>
            </NavItem>
            <NavItem className='line'>
              <NavLink style={{ color:'white' }} href='/logged_in_quote'>Quote</NavLink>
            </NavItem>
            <NavItem>
              <NavLink style={{ color:'white' }} href='/logged_in_price_history'>Price History (restricted)</NavLink>
            </NavItem>
            <NavItem className='navbar-space-right'>
              <NavLink className='logout-space-right' style={{ color:'white'}} href='/'>Logout</NavLink>
            </NavItem>           
          </Nav>
        </Navbar>
      </div>
      <div className='container'>
        <br></br>
        <div
        className='ag-theme-balham'
        style= {{ height: '300px', width: '1000px' }}>
          <h6 style={{ color:'yellow' }}>Search date from:</h6>
          <span>
            <input type="date"></input>
            <button>Search</button>
          </span> 
          <br></br>
          <br></br>    
          <br></br>
          <input 
            value={rowSymbol}
            onChange={(e) => setRowSymbol(e.target.value)}
            placeholder= "Search stock symbol..." >
          </input>
          <AgGridReact
          columnDefs={columns}
          rowData={rowData} />
        </div>                     
      </div>
    </div>   
  );
}

export default LoggedInPriceHistory;