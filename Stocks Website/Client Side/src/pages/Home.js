import React, { useState, useEffect } from 'react';
import { Navbar, Nav, NavLink, NavItem, Badge } from 'reactstrap';
import { AgGridReact } from 'ag-grid-react';
import 'ag-grid-community/dist/styles/ag-grid.css';
import 'ag-grid-community/dist/styles/ag-theme-balham.css'; 


function Home(props) {
  const [rowData, setRowData] = useState([]);

  const columns = [
    { headerName: 'Name', field: 'name', sortable: true },
    { headerName: 'Symbol', field: 'symbol', sortable: true },
    { headerName: 'Industry', field: 'industry', sortable: true, filter: true }
  ];

  useEffect(() => {
    fetch('http://131.181.190.87:3000/stocks/symbols')
    .then(response => { response.json()
      .then(stocks => { setRowData(stocks)
      })
    });    
  }, []);


  return (
    <div className='App-header Home-header'>
      <div>
        <Navbar expand='md'>
          <Nav className='Navbar' navbar>            
            <NavItem className='line'>
              <NavLink style={{ color:'white' }} href='/'>Home</NavLink>
            </NavItem>
            <NavItem className='line'>
              <NavLink style={{ color:'white' }} href='/quote'>Quote</NavLink>
            </NavItem>
            <NavItem>
              <NavLink style={{ color:'white' }} href='/price_history'>Price History (restricted)</NavLink>
            </NavItem>
            <NavItem className='navbar-space-right'>
              <NavLink className='line' style={{ color:'white'}} href='/login'>Login</NavLink>
            </NavItem>
            <NavItem>
              <NavLink style={{ color:'white' }} href='/login'>Register</NavLink>
            </NavItem>            
          </Nav>
        </Navbar>
      </div>
      <br></br>
      <p style={{ color:'yellow' }}>
        Welcome to the <b>Rested QUT Stock</b> portal, here we provide you with a vast range <br></br>
        of stocks provided by the REST API. Click on 'Quote' to get a detailed list of stock <br></br>
        entries on a specified symbol, or 'Price History' to view a particular range of dates <br></br>
        on these given stock entries. To create or login to a pre-existing account please <br></br>
        click on either 'Login' or 'Register'.
      </p>
      <div className='container'>
        <br></br>
        <h4>Stock List</h4>
        <p>
          <Badge color='success'>{rowData.length}</Badge> Total stocks
        </p>
        <div        
        className='ag-theme-balham'
        style= {{ height: '500px', width: '600px' }}>
          <AgGridReact
          columnDefs={columns}
          rowData={rowData}
          pagination={true}
          paginationPageSize={20} />
        </div>           
      </div>
    </div>    
  );
}

export default Home;